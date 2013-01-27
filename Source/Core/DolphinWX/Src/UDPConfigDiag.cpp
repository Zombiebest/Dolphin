#include "UDPConfigDiag.h"
#include "UDPWrapper.h"

#include "Common.h"
#include "ControllerEmu.h"
#include "IniFile.h"
#include <string>

UDPConfigDiag::UDPConfigDiag(wxWindow * const parent, UDPWrapper * _wrp) :
	wxDialog(parent, -1, _("UDP Wiimote"), wxDefaultPosition, wxDefaultSize),
	wrp(_wrp)
{
	wxBoxSizer *const outer_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *const sizer1 = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer *const sizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Update"));

	outer_sizer->Add(sizer1, 0, wxTOP | wxLEFT | wxRIGHT | wxEXPAND, 5);
	outer_sizer->Add(sizer2, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

	enable = new wxCheckBox(this, wxID_ANY, _("Enable"));
	butt = new wxCheckBox(this, wxID_ANY, _("Buttons"));
	accel = new wxCheckBox(this, wxID_ANY, _("Acceleration"));
	point = new wxCheckBox(this, wxID_ANY, _("IR Pointer"));
	nun = new wxCheckBox(this, wxID_ANY, _("Nunchuk"));
	nunaccel = new wxCheckBox(this, wxID_ANY, _("Nunchuk Acceleration"));

	wxBoxSizer *const port_sizer = new wxBoxSizer(wxHORIZONTAL);
	port_sizer->Add(new wxStaticText(this, wxID_ANY, _("UDP Port:")), 0, wxALIGN_CENTER);
	port_tbox = new wxTextCtrl(this, wxID_ANY, wxString::FromUTF8(wrp->port.c_str()));
	port_sizer->Add(port_tbox, 1, wxLEFT | wxEXPAND, 5);

	enable->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &UDPConfigDiag::ChangeState, this);
	butt->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &UDPConfigDiag::ChangeUpdateFlags, this);
	accel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &UDPConfigDiag::ChangeUpdateFlags, this);
	point->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &UDPConfigDiag::ChangeUpdateFlags, this);
	nun->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &UDPConfigDiag::ChangeUpdateFlags, this);
	nunaccel->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &UDPConfigDiag::ChangeUpdateFlags, this);
	port_tbox->Bind(wxEVT_COMMAND_TEXT_UPDATED, &UDPConfigDiag::ChangeState, this);

	enable->SetValue(wrp->udpEn);
	butt->SetValue(wrp->updButt);
	accel->SetValue(wrp->updAccel);
	point->SetValue(wrp->updIR);
	nun->SetValue(wrp->updNun);
	nunaccel->SetValue(wrp->updNunAccel);
	
	sizer1->Add(enable, 1, wxALL | wxEXPAND, 5);
	sizer1->Add(port_sizer, 1, wxBOTTOM | wxLEFT| wxRIGHT | wxEXPAND, 5);

	sizer2->Add(butt, 1, wxALL | wxEXPAND, 5);
	sizer2->Add(accel, 1, wxALL | wxEXPAND, 5);
	sizer2->Add(point, 1, wxALL | wxEXPAND, 5);
	sizer2->Add(nun, 1, wxALL | wxEXPAND, 5);
	sizer2->Add(nunaccel, 1, wxALL | wxEXPAND, 5);

	outer_sizer->Add(CreateButtonSizer(wxOK), 0, wxALL | wxALIGN_RIGHT, 5);

	SetSizerAndFit(outer_sizer);
	Center();
	SetFocus();
}

void UDPConfigDiag::ChangeUpdateFlags(wxCommandEvent & WXUNUSED(event))
{
	wrp->updAccel=accel->GetValue();
	wrp->updButt=butt->GetValue();
	wrp->updIR=point->GetValue();
	wrp->updNun=nun->GetValue();
	wrp->updNunAccel=nunaccel->GetValue();
}

void UDPConfigDiag::ChangeState(wxCommandEvent & WXUNUSED(event))
{
	wrp->udpEn=enable->GetValue();
	wrp->port=port_tbox->GetValue().mb_str(wxConvUTF8);
	wrp->Refresh();
}
