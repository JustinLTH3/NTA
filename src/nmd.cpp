//
// Created by JustinLai on 23/3/2026.
//

#include "nmd.h"

#include <QMap>

#include "md4c-html.h"
#include <QStringBuilder>
#include <spdlog/spdlog.h>

extern "C" {
#include "entity.h"
}

namespace NTA
{
    const MD_CHAR* nmd::openHead[6] = {"<h1>", "<h2>", "<h3>", "<h4>", "<h5>", "<h6>"};
    const MD_CHAR* nmd::closeHead[6] = {"</h1>\n", "</h2>\n", "</h3>\n", "</h4>\n", "</h5>\n", "</h6>\n"};

    nmd::nmd()
    {
        md.abi_version = 0;

        md.flags = MD_DIALECT_GITHUB | MD_FLAG_WIKILINKS | MD_FLAG_LATEXMATHSPANS;

        md.enter_block = [](MD_BLOCKTYPE t, void* d, void* u) { return ((nmd*)u)->enter_block_callback(t, d, u); };
        md.leave_block = [](MD_BLOCKTYPE t, void* d, void* u) { return ((nmd*)u)->leave_block_callback(t, d, u); };
        md.enter_span = [](MD_SPANTYPE t, void* d, void* u) { return ((nmd*)u)->enter_span_callback(t, d, u); };
        md.leave_span = [](MD_SPANTYPE t, void* d, void* u) { return ((nmd*)u)->leave_span_callback(t, d, u); };
        md.text = [](MD_TEXTTYPE t, const MD_CHAR* text, MD_SIZE size, void* u)
        {
            return ((nmd*)u)->text_callback(t, text, size, u);
        };
        md.debug_log = [](const char* msg, void*)
        {
            spdlog::info("{}", msg);
        };
        md.syntax = nullptr;

        /* Build map of characters which need escaping. */
        for (int i = 0; i < 256; i++)
        {
            const auto ch = static_cast<unsigned char>(i);
            escape_map[i] = 0;
            if (strchr("\"&<>", ch) != nullptr)
                escape_map[i] |= NEED_HTML_ESC_FLAG;

            if (!isalnum(ch) && strchr("~-_.+!*(),%#@?=;:/,+$", ch) == nullptr)
                escape_map[i] |= NEED_URL_ESC_FLAG;
        }
    }

    int nmd::md2html(const QString& input, QString& result)
    {
        if (input.isEmpty())return 0;
        auto i = input.toUtf8();
        auto r = md_parse(i, MD_SIZE(i.size()), &md, this);
        if (!r)result = QString::fromUtf8(temp);
        temp.clear();
        return r;
    }


    void nmd::append(const char* s)
    {
        temp.append(s);
    }

    void nmd::appendWithSize(const MD_CHAR* s, MD_SIZE n)
    {
        if (n > 0)
            temp.append(s, static_cast<int>(n));
    }


    int nmd::enter_block_callback(MD_BLOCKTYPE type, void* detail, void* userdata)
    {
        switch (type)
        {
        case MD_BLOCK_DOC:
            break;
        case MD_BLOCK_QUOTE:
            append("<blockquote>\n");
            break;
        case MD_BLOCK_UL:
            append("<ul>\n");
            break;
        case MD_BLOCK_OL:
            render_open_ol_block(static_cast<const MD_BLOCK_OL_DETAIL*>(detail));
            break;
        case MD_BLOCK_LI:
            render_open_li_block(static_cast<const MD_BLOCK_LI_DETAIL*>(detail));
            break;
        case MD_BLOCK_HR:
            append("<hr>\n");
            break;
        case MD_BLOCK_H:
            append(openHead[static_cast<MD_BLOCK_H_DETAIL*>(detail)->level - 1]);
            break;
        case MD_BLOCK_CODE:
            render_open_code_block(static_cast<const MD_BLOCK_CODE_DETAIL*>(detail));
            break;
        case MD_BLOCK_HTML:
            break;
        case MD_BLOCK_P:
            append("<p>");
            break;
        case MD_BLOCK_TABLE:
            append("<table>\n");
            break;
        case MD_BLOCK_THEAD:
            append("<thead>\n");
            break;
        case MD_BLOCK_TBODY:
            append("<tbody>\n");
            break;
        case MD_BLOCK_TR:
            append("<tr>\n");
            break;
        case MD_BLOCK_TH:
            render_open_td_block("th", static_cast<MD_BLOCK_TD_DETAIL*>(detail));
            break;
        case MD_BLOCK_TD:
            render_open_td_block("td", static_cast<MD_BLOCK_TD_DETAIL*>(detail));
            break;
        }

        return 0;
    }

    int nmd::leave_block_callback(MD_BLOCKTYPE type, void* detail, void* userdata)
    {
        switch (type)
        {
        case MD_BLOCK_DOC:
            break;
        case MD_BLOCK_QUOTE:
            append("</blockquote>\n");
            break;
        case MD_BLOCK_UL:
            append("</ul>\n");
            break;
        case MD_BLOCK_OL:
            append("</ol>\n");
            break;
        case MD_BLOCK_LI:
            append("</li>\n");
            break;
        case MD_BLOCK_HR:
            break;
        case MD_BLOCK_H:
            append(closeHead[static_cast<MD_BLOCK_H_DETAIL*>(detail)->level - 1]);
            break;
        case MD_BLOCK_CODE:
            append("</code></pre>\n");
            break;
        case MD_BLOCK_HTML:
            break;
        case MD_BLOCK_P:
            append("</p>\n");
            break;
        case MD_BLOCK_TABLE:
            append("</table>\n");
            break;
        case MD_BLOCK_THEAD:
            append("</thead>\n");
            break;
        case MD_BLOCK_TBODY: append("</tbody>\n");
            break;
        case MD_BLOCK_TR: append("</tr>\n");
            break;
        case MD_BLOCK_TH: append("</th>\n");
            break;
        case MD_BLOCK_TD: append("</td>\n");
            break;
        }

        return 0;
    }

    int nmd::enter_span_callback(MD_SPANTYPE type, void* detail, void* userdata)
    {
        int inside_img = (image_nesting_level > 0);

        /* We are inside a Markdown image label. Markdown allows to use any emphasis
         * and other rich contents in that context similarly as in any link label.
         *
         * However, unlike in the case of links (where that contents becomescontents
         * of the <a>...</a> tag), in the case of images the contents is supposed to
         * fall into the attribute alt: <img alt="...">.
         *
         * In that context we naturally cannot output nested HTML tags. So lets
         * suppress them and only output the plain text (i.e. what falls into text()
         * callback).
         *
         * CommonMark specification declares this a recommended practice for HTML
         * output.
         */
        if (type == MD_SPAN_IMG)
            image_nesting_level++;
        if (inside_img)
            return 0;

        switch (type)
        {
        case MD_SPAN_EM: append("<em>");
            break;
        case MD_SPAN_STRONG: append("<strong>");
            break;
        case MD_SPAN_U: append("<u>");
            break;
        case MD_SPAN_A: render_open_a_span(static_cast<MD_SPAN_A_DETAIL*>(detail));
            break;
        case MD_SPAN_IMG: render_open_img_span(static_cast<MD_SPAN_IMG_DETAIL*>(detail));
            break;
        case MD_SPAN_CODE: append("<code>");
            break;
        case MD_SPAN_DEL: append("<del>");
            break;
        case MD_SPAN_LATEXMATH: append("<x-equation>");
            break;
        case MD_SPAN_LATEXMATH_DISPLAY: append("<x-equation type=\"display\">");
            break;
        case MD_SPAN_WIKILINK: render_open_wikilink_span(static_cast<MD_SPAN_WIKILINK_DETAIL*>(detail));
            break;
        }

        return 0;
    }

    int nmd::leave_span_callback(MD_SPANTYPE type, void* detail, void* userdata)
    {
        if (type == MD_SPAN_IMG)
            image_nesting_level--;
        if (image_nesting_level > 0)
            return 0;

        switch (type)
        {
        case MD_SPAN_EM: append("</em>");
            break;
        case MD_SPAN_STRONG: append("</strong>");
            break;
        case MD_SPAN_U: append("</u>");
            break;
        case MD_SPAN_A: append("</a>");
            break;
        case MD_SPAN_IMG: render_close_img_span(static_cast<MD_SPAN_IMG_DETAIL*>(detail));
            break;
        case MD_SPAN_CODE: append("</code>");
            break;
        case MD_SPAN_DEL: append("</del>");
            break;
        case MD_SPAN_LATEXMATH: /*fall through*/
        case MD_SPAN_LATEXMATH_DISPLAY: append("</x-equation>");
            break;
        case MD_SPAN_WIKILINK: append("</a>");
            break;
        }

        return 0;
    }

    int nmd::text_callback(MD_TEXTTYPE type, const MD_CHAR* text, MD_SIZE size, void* userdata)
    {
        switch (type)
        {
        case MD_TEXT_NULLCHAR: render_utf8_codepoint(0x0000, &nmd::appendWithSize);
            break;
        case MD_TEXT_BR: append(image_nesting_level == 0 ? "<br>\n" : " ");
            break;
        case MD_TEXT_SOFTBR: append(image_nesting_level == 0 ? "\n" : " ");
            break;
        case MD_TEXT_HTML: appendWithSize(text, size);
            break;
        case MD_TEXT_ENTITY: render_entity(text, size, &nmd::render_html_escaped);
            break;
        default: render_html_escaped(text, size);
            break;
        }

        return 0;
    }

    void nmd::render_attribute(const MD_ATTRIBUTE* attr, void (nmd::*fn_append)(const MD_CHAR*, MD_SIZE))
    {
        int i;

        for (i = 0; attr->substr_offsets[i] < attr->size; i++)
        {
            MD_TEXTTYPE type = attr->substr_types[i];
            MD_OFFSET off = attr->substr_offsets[i];
            MD_SIZE size = attr->substr_offsets[i + 1] - off;
            const MD_CHAR* text = attr->text + off;

            switch (type)
            {
            case MD_TEXT_NULLCHAR: render_utf8_codepoint(0x0000, &nmd::appendWithSize);
                break;
            case MD_TEXT_ENTITY: render_entity(text, size, fn_append);
                break;
            default: (this->*fn_append)(text, size);
                break;
            }
        }
    }

    void nmd::render_utf8_codepoint(unsigned codepoint, void (nmd::*fn_append)(const MD_CHAR*, MD_SIZE))
    {
        static const MD_CHAR utf8_replacement_char[] = {(char)0xef, (char)0xbf, (char)0xbd};

        unsigned char utf8[4];
        size_t n;

        if (codepoint <= 0x7f)
        {
            n = 1;
            utf8[0] = codepoint;
        }
        else if (codepoint <= 0x7ff)
        {
            n = 2;
            utf8[0] = 0xc0 | ((codepoint >> 6) & 0x1f);
            utf8[1] = 0x80 + ((codepoint >> 0) & 0x3f);
        }
        else if (codepoint <= 0xffff)
        {
            n = 3;
            utf8[0] = 0xe0 | ((codepoint >> 12) & 0xf);
            utf8[1] = 0x80 + ((codepoint >> 6) & 0x3f);
            utf8[2] = 0x80 + ((codepoint >> 0) & 0x3f);
        }
        else
        {
            n = 4;
            utf8[0] = 0xf0 | ((codepoint >> 18) & 0x7);
            utf8[1] = 0x80 + ((codepoint >> 12) & 0x3f);
            utf8[2] = 0x80 + ((codepoint >> 6) & 0x3f);
            utf8[3] = 0x80 + ((codepoint >> 0) & 0x3f);
        }

        if (0 < codepoint && codepoint <= 0x10ffff)
            (this->*fn_append)((char*)utf8, (MD_SIZE)n);
        else
            (this->*fn_append)(utf8_replacement_char, 3);
    }


    void nmd::render_url_escaped(const MD_CHAR* data, MD_SIZE size)
    {
        static const MD_CHAR hex_chars[] = "0123456789ABCDEF";
        MD_OFFSET beg = 0;
        MD_OFFSET off = 0;

        /* Some characters need to be escaped in URL attributes. */
#define NEED_URL_ESC(ch)    (escape_map[(unsigned char)(ch)] & NEED_URL_ESC_FLAG)

        while (1)
        {
            while (off < size && !NEED_URL_ESC(data[off]))
                off++;
            if (off > beg)
                appendWithSize(data + beg, off - beg);

            if (off < size)
            {
                char hex[3];

                switch (data[off])
                {
                case '&': append("&amp;");
                    break;
                default:
                    hex[0] = '%';
                    hex[1] = hex_chars[((unsigned)data[off] >> 4) & 0xf];
                    hex[2] = hex_chars[((unsigned)data[off] >> 0) & 0xf];
                    appendWithSize(hex, 3);
                    break;
                }
                off++;
            }
            else
            {
                break;
            }

            beg = off;
        }
    }

    void nmd::render_open_a_span(const MD_SPAN_A_DETAIL* det)
    {
        append("<a href=\"");
        render_attribute(&det->href, &nmd::render_url_escaped);

        if (det->title.text != nullptr)
        {
            append("\" title=\"");
            render_attribute(&det->title, &nmd::render_html_escaped);
        }

        append("\">");
    }

    unsigned hex_val(char ch)
    {
        if ('0' <= ch && ch <= '9')
            return ch - '0';
        if ('A' <= ch && ch <= 'Z')
            return ch - 'A' + 10;
        return ch - 'a' + 10;
    }

    void nmd::render_entity(const MD_CHAR* text, MD_SIZE size, void (nmd::*fn_append)(const MD_CHAR*, MD_SIZE))
    {
        /* We assume UTF-8 output is what is desired. */
        if (size > 3 && text[1] == '#')
        {
            unsigned codepoint = 0;

            if (text[2] == 'x' || text[2] == 'X')
            {
                /* Hexadecimal entity (e.g. "&#x1234abcd;")). */
                MD_SIZE i;
                for (i = 3; i < size - 1; i++)
                    codepoint = 16 * codepoint + hex_val(text[i]);
            }
            else
            {
                /* Decimal entity (e.g. "&1234;") */
                MD_SIZE i;
                for (i = 2; i < size - 1; i++)
                    codepoint = 10 * codepoint + (text[i] - '0');
            }

            render_utf8_codepoint(codepoint, fn_append);
            return;
        }
        else
        {
            /* Named entity (e.g. "&nbsp;"). */
            const ENTITY* ent;

            ent = entity_lookup(text, size);
            if (ent != NULL)
            {
                render_utf8_codepoint(ent->codepoints[0], fn_append);
                if (ent->codepoints[1])
                    render_utf8_codepoint(ent->codepoints[1], fn_append);
                return;
            }
        }

        (this->*fn_append)(text, size);
    }

    void nmd::render_html_escaped(const MD_CHAR* data, MD_SIZE size)
    {
        MD_OFFSET beg = 0;
        MD_OFFSET off = 0;

        /* Some characters need to be escaped in normal HTML text. */
#define NEED_HTML_ESC(ch)   (escape_map[(unsigned char)(ch)] & NEED_HTML_ESC_FLAG)

        while (true)
        {
            /* Optimization: Use some loop unrolling. */
            while (off + 3 < size && !NEED_HTML_ESC(data[off+0]) && !NEED_HTML_ESC(data[off+1])
                && !NEED_HTML_ESC(data[off+2]) && !NEED_HTML_ESC(data[off+3]))
                off += 4;
            while (off < size && !NEED_HTML_ESC(data[off]))
                off++;

            if (off > beg)
                appendWithSize(data + beg, off - beg);

            if (off < size)
            {
                switch (data[off])
                {
                case '&': append("&amp;");
                    break;
                case '<': append("&lt;");
                    break;
                case '>': append("&gt;");
                    break;
                case '"': append("&quot;");
                    break;
                default:
                    appendWithSize(data + off, 1);
                    break;
                }
                off++;
            }
            else
            {
                break;
            }
            beg = off;
        }
    }

    void nmd::render_open_img_span(const MD_SPAN_IMG_DETAIL* det)
    {
        append("<img src=\"");
        render_attribute(&det->src, &nmd::render_url_escaped);
        append("\" alt=\"");
    }

    void nmd::render_open_wikilink_span(const MD_SPAN_WIKILINK_DETAIL* det)
    {
        append("<a href=\"nta:");
        render_attribute(&det->target, &nmd::render_url_escaped);
        append("\">");
    }

    void nmd::render_close_img_span(const MD_SPAN_IMG_DETAIL* det)
    {
        if (det->title.text != NULL)
        {
            append("\" title=\"");
            render_attribute(&det->title, &nmd::render_html_escaped);
        }

        append("\" />");
    }

    void nmd::render_open_code_block(const MD_BLOCK_CODE_DETAIL* det)
    {
        append("<pre><code");

        /* If known, output the HTML 5 attribute class="language-LANGNAME". */
        if (det->lang.text != nullptr)
        {
            append(" class=\"language-");
            render_attribute(&det->lang, &nmd::render_html_escaped);
            append("\"");
        }

        append(">");
    }

    void nmd::render_open_td_block(const MD_CHAR* cell_type, const MD_BLOCK_TD_DETAIL* det)
    {
        append("<");
        append(cell_type);

        switch (det->align)
        {
        case MD_ALIGN_LEFT: append(" align=\"left\">");
            break;
        case MD_ALIGN_CENTER: append(" align=\"center\">");
            break;
        case MD_ALIGN_RIGHT: append(" align=\"right\">");
            break;
        default: append(">");
            break;
        }
    }

    void nmd::render_open_ol_block(const MD_BLOCK_OL_DETAIL* det)
    {
        char buf[64];

        if (det->start == 1)
        {
            append("<ol>\n");
            return;
        }

        snprintf(buf, sizeof(buf), "<ol start=\"%u\">\n", det->start);
        append(buf);
    }

    void nmd::render_open_li_block(const MD_BLOCK_LI_DETAIL* det)
    {
        if (det->is_task)
        {
            append("<li class=\"task-list-item\">"
                "<input type=\"checkbox\" class=\"task-list-item-checkbox\" disabled");
            if (det->task_mark == 'x' || det->task_mark == 'X')
                append(" checked");
            append(">");
        }
        else
        {
            append("<li>");
        }
    }
} // NTA
