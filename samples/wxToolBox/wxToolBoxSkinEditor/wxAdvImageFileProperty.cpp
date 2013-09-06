#include "AdditionalProperties.h"

// -----------------------------------------------------------------------
// AdvImageFile Property
// -----------------------------------------------------------------------

class wxMyImageInfo;

WX_DECLARE_OBJARRAY(wxMyImageInfo, wxArrayMyImageInfo);

class wxMyImageInfo
{
public:
    wxString    m_path;
    wxBitmap*   m_pThumbnail1; // smaller thumbnail
    wxBitmap*   m_pThumbnail2; // larger thumbnail

    wxMyImageInfo ( const wxString& str )
    {
        m_path = str;
        m_pThumbnail1 = (wxBitmap*) NULL;
        m_pThumbnail2 = (wxBitmap*) NULL;
    }
    ~wxMyImageInfo()
    {
        if ( m_pThumbnail1 )
            delete m_pThumbnail1;
        if ( m_pThumbnail2 )
            delete m_pThumbnail2;
    }

};


#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxArrayMyImageInfo);

wxArrayString       g_myImageNames;
wxArrayMyImageInfo  g_myImageArray;


// Preferred thumbnail height.
#define PREF_THUMBNAIL_HEIGHT       64


class wxAdvImageFilePropertyClass : public wxFilePropertyClass
{
    WX_PG_DECLARE_PROPERTY_CLASS()
public:

    wxAdvImageFilePropertyClass( const wxString& label, const wxString& name,
        const wxString& value );
    virtual ~wxAdvImageFilePropertyClass ();

    void DoSetValue ( wxPGVariant value ); // Override to allow image loading.

    WX_PG_DECLARE_CHOICE_METHODS()
    WX_PG_DECLARE_EVENT_METHODS()
    WX_PG_DECLARE_CUSTOM_PAINT_METHODS()

    void LoadThumbnails ( size_t n );

protected:
    wxImage*    m_pImage; // Temporary thumbnail data.
    
    int m_index; // Index required for choice behaviour.
};


WX_PG_IMPLEMENT_PROPERTY_CLASS(wxAdvImageFileProperty,wxFileProperty,
                               wxString,const wxString&,ChoiceAndButton)


wxAdvImageFilePropertyClass::wxAdvImageFilePropertyClass ( const wxString& label,
    const wxString& name, const wxString& value)
    : wxFilePropertyClass(label,name,value)
{
    m_wildcard = wxPGGetDefaultImageWildcard();

    m_index = -1;

    m_pImage = (wxImage*) NULL;

    // Only show names.
    m_flags &= ~(wxPG_PROP_SHOW_FULL_FILENAME);
}

wxAdvImageFilePropertyClass::~wxAdvImageFilePropertyClass ()
{
    // Delete old image
    if ( m_pImage )
    {
        delete m_pImage;
        m_pImage = (wxImage*) NULL;
    }
}

void wxAdvImageFilePropertyClass::DoSetValue ( wxPGVariant value )
{
    wxFilePropertyClass::DoSetValue(value);

    // Delete old image
    if ( m_pImage )
    {
        delete m_pImage;
        m_pImage = (wxImage*) NULL;
    }

    wxString imagename = GetValueAsString(0);

    if ( imagename.length() )
    {

        int index = g_myImageNames.Index(imagename);

        // If not in table, add now.
        if ( index == wxNOT_FOUND )
        {
            g_myImageNames.Add( imagename );
            g_myImageArray.Add( new wxMyImageInfo ( m_filename.GetFullPath() ) );

            index = g_myImageArray.GetCount() - 1;
        }

        // If no thumbnail ready, then need to load image.
        if ( !g_myImageArray[index].m_pThumbnail2 )
        {
            // Load if file exists.
            if ( m_filename.FileExists() )
                m_pImage = new wxImage ( m_filename.GetFullPath() );
        }

        m_index = index;

    }
    else
        m_index = -1;

}

int wxAdvImageFilePropertyClass::GetChoiceInfo ( wxPGChoiceInfo* choiceinfo )
{
    if ( choiceinfo )
    {
        choiceinfo->m_itemCount = g_myImageNames.GetCount();
        if ( g_myImageNames.GetCount() )
            choiceinfo->m_arrWxString = &g_myImageNames.Item(0);
    }
    return m_index;
}

bool wxAdvImageFilePropertyClass::SetValueFromInt ( long value, int /*flags*/ )
{
    wxASSERT ( value >= 0 );
    m_index = value;
    return SetValueFromString( g_myImageNames.Item(value), wxPG_FULL_VALUE );
}

bool wxAdvImageFilePropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxWindow* primary,
   wxEvent& event )
{
    if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
    {
        size_t prev_itemcount = g_myImageArray.GetCount();

        bool res = wxFilePropertyClass::OnEvent(propgrid,primary,event);

        if ( res )
        {
            wxString name = GetValueAsString(0);

            if ( g_myImageArray.GetCount() != prev_itemcount )
            {
                wxASSERT ( g_myImageArray.GetCount() == (prev_itemcount+1) );

                // Add to the control's array.
                // (should be added to own array earlier)

                if ( primary )
                    GetEditorClass()->InsertItem(primary,name, 0);

            }

            if ( primary )
               GetEditorClass()->UpdateControl(this,primary);

            return TRUE;
        }

    }
    return FALSE;
}

wxSize wxAdvImageFilePropertyClass::GetImageSize() const
{
    return wxPG_FLEXIBLE_SIZE(PREF_THUMBNAIL_HEIGHT,PREF_THUMBNAIL_HEIGHT);
}

void wxAdvImageFilePropertyClass::LoadThumbnails ( size_t index )
{
    wxMyImageInfo& mii = g_myImageArray[index];

    if ( !mii.m_pThumbnail2 )
    {

        if ( !m_pImage || !m_pImage->Ok() ||
             m_filename != mii.m_path
           )
        {
            if ( m_pImage )
                delete m_pImage;
            m_pImage = new wxImage( mii.m_path );
        }

        if ( m_pImage && m_pImage->Ok() )
        {
            int im_wid = m_pImage->GetWidth();
            int im_hei = m_pImage->GetHeight();
            if ( im_hei > PREF_THUMBNAIL_HEIGHT )
            {
                // TNW = (TNH*IW)/IH
                im_wid = (PREF_THUMBNAIL_HEIGHT*m_pImage->GetWidth())/m_pImage->GetHeight();
                im_hei = PREF_THUMBNAIL_HEIGHT;
            }

            m_pImage->Rescale ( im_wid, im_hei );

            mii.m_pThumbnail2 = new wxBitmap ( *m_pImage );

            wxSize cis = GetParentState()->GetGrid()->GetImageSize();
            m_pImage->Rescale ( cis.x, cis.y );

            mii.m_pThumbnail1 = new wxBitmap ( *m_pImage );

        }

        if ( m_pImage )
        {
            delete m_pImage;
            m_pImage = (wxImage*) NULL;
        }
    }
}

void wxAdvImageFilePropertyClass::OnCustomPaint ( wxDC& dc,
    const wxRect& rect, wxPGPaintData& pd )
{
    int index = m_index;
    if ( pd.m_choiceItem >= 0 )
        index = pd.m_choiceItem;

    //wxLogDebug(wxT("%i"),index);

    if ( index >= 0 )
    {
        LoadThumbnails(index);

        // Is this a measure item call?
        if ( rect.x < 0 )
        {
            // Variable height
            //pd.m_drawnHeight = PREF_THUMBNAIL_HEIGHT;
            wxBitmap* pBitmap = (wxBitmap*)g_myImageArray[index].m_pThumbnail2;
            if ( pBitmap )
                pd.m_drawnHeight = pBitmap->GetHeight();
            else
                pd.m_drawnHeight = 16;
            return;
        }

        // Draw the thumbnail

        wxBitmap* pBitmap;

        if ( pd.m_choiceItem >= 0 )
            pBitmap = (wxBitmap*)g_myImageArray[index].m_pThumbnail2;
        else
            pBitmap = (wxBitmap*)g_myImageArray[index].m_pThumbnail1;

        if ( pBitmap )
        {
            dc.DrawBitmap ( *pBitmap, rect.x, rect.y, FALSE );

            // Tell the caller how wide we drew.
            pd.m_drawnWidth = pBitmap->GetWidth();

            return;
        }
    }

    // No valid file - just draw a white box.
    dc.SetBrush ( *wxWHITE_BRUSH );
    dc.DrawRectangle ( rect );
}
