//
// Created by JustinLai on 23/3/2026.
//

#pragma once
#include <QString>

#include "md4c.h"

namespace NTA
{
#define NEED_HTML_ESC_FLAG   0x1
#define NEED_URL_ESC_FLAG    0x2

    class nmd
    {
    public:
        nmd();
        virtual ~nmd() = default;
        int md2html(const QString& input, QString& result);

    protected:
        inline void append(const char* s);
        inline void appendWithSize(const MD_CHAR* s, MD_SIZE n);

        virtual int enter_block_callback(MD_BLOCKTYPE type, void* detail, void* userdata);
        virtual int leave_block_callback(MD_BLOCKTYPE type, void* detail, void* userdata);
        virtual int enter_span_callback(MD_SPANTYPE type, void* detail, void* userdata);
        virtual int leave_span_callback(MD_SPANTYPE type, void* detail, void* userdata);
        virtual int text_callback(MD_TEXTTYPE type, const MD_CHAR* text, MD_SIZE size, void* userdata);

        void render_attribute(const MD_ATTRIBUTE* attr, void (nmd::*fn_append)(const MD_CHAR*, MD_SIZE));
        void render_utf8_codepoint(unsigned codepoint, void (nmd::*fn_append)(const MD_CHAR*, MD_SIZE));
        void render_url_escaped(const MD_CHAR* data, MD_SIZE size);
        void render_open_a_span(const MD_SPAN_A_DETAIL* det);
        void render_entity(const MD_CHAR* text, MD_SIZE size, void (nmd::*fn_append)(const MD_CHAR*, MD_SIZE));
        void render_html_escaped(const MD_CHAR* data, MD_SIZE size);
        void render_open_img_span(const MD_SPAN_IMG_DETAIL* det);
        void render_open_wikilink_span(const MD_SPAN_WIKILINK_DETAIL* det);
        void render_close_img_span(const MD_SPAN_IMG_DETAIL* det);
        void render_open_code_block(const MD_BLOCK_CODE_DETAIL* det);
        void render_open_td_block(const MD_CHAR* cell_type, const MD_BLOCK_TD_DETAIL* det);
        void render_open_ol_block(const MD_BLOCK_OL_DETAIL* det);
        void render_open_li_block(const MD_BLOCK_LI_DETAIL* det);

        static const MD_CHAR* openHead[6];
        static const MD_CHAR* closeHead[6];
        int image_nesting_level = 0;

    private:
        QByteArray temp;
        MD_PARSER md;
        char escape_map[256];
    };
} // NTA
