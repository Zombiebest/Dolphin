/////////////////////////////////////////////////////////////////////////////
// Name:        wx/imagtga.h
// Purpose:     wxImage TGA handler
// Author:      Seth Jackson
// RCS-ID:      $Id: imagtga.h 59461 2009-03-09 23:13:34Z VZ $
// Copyright:   (c) 2005 Seth Jackson
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGTGA_H_
#define _WX_IMAGTGA_H_

#include "wx/image.h"

//-----------------------------------------------------------------------------
// wxTGAHandler
//-----------------------------------------------------------------------------

#if wxUSE_TGA

class WXDLLIMPEXP_CORE wxTGAHandler : public wxImageHandler
{
public:
    wxTGAHandler()
    {
        m_name = wxT("TGA file");
        m_extension = wxT("tga");
        m_altExtensions.Add(wxT("tpic"));
        m_type = wxBITMAP_TYPE_TGA;
        m_mime = wxT("image/tga");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile(wxImage* image, wxInputStream& stream,
                            bool verbose = true, int index = -1);
    virtual bool SaveFile(wxImage* image, wxOutputStream& stream,
                             bool verbose = true);
protected:
    virtual bool DoCanRead(wxInputStream& stream);
#endif // wxUSE_STREAMS

    DECLARE_DYNAMIC_CLASS(wxTGAHandler)
};

#endif // wxUSE_TGA

#endif // _WX_IMAGTGA_H_
