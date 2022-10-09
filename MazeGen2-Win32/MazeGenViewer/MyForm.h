#pragma once
#include "OpenGLWin.h"

namespace MazeGenViewer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			glView = gcnew OpenGLWin(this->splitContainer1->Panel1);
			
			gridColorButton->BackColor = glView->gridColor;
			wallColorButton->BackColor = glView->wallColor;
			pathColorButton->BackColor = glView->pathColor;
			decayColorButton->BackColor = glView->decayColor;
			//AllocConsole();
					
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			delete bldr;
			if (components)
			{
				delete components;
				
			}

		}


	private: OpenGLForm::OpenGLWin ^ glView;
	private: MazeBuilder* bldr;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Button^  gridColorButton;
	private: System::Windows::Forms::Button^  wallColorButton;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  pathColorButton;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::CheckBox^  fullPathCB;
	private: System::Windows::Forms::Button^  decayColorButton;
	private: System::Windows::Forms::CheckBox^  decayPathCB;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  decaySpinner;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::CheckBox^  rotateCB;
	private: System::Windows::Forms::Button^  refreshButton;
	private: System::Windows::Forms::ColorDialog^  colorDialog1;

	private: System::ComponentModel::IContainer^  components;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->gridColorButton = (gcnew System::Windows::Forms::Button());
			this->refreshButton = (gcnew System::Windows::Forms::Button());
			this->decayColorButton = (gcnew System::Windows::Forms::Button());
			this->decayPathCB = (gcnew System::Windows::Forms::CheckBox());
			this->wallColorButton = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->pathColorButton = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->fullPathCB = (gcnew System::Windows::Forms::CheckBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->decaySpinner = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->rotateCB = (gcnew System::Windows::Forms::CheckBox());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->decaySpinner))->BeginInit();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 17;
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->IsSplitterFixed = true;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Resize += gcnew System::EventHandler(this, &MyForm::splitContainer1_Panel1_Resize);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->BackColor = System::Drawing::SystemColors::Control;
			this->splitContainer1->Panel2->Controls->Add(this->tableLayoutPanel1);
			this->splitContainer1->Size = System::Drawing::Size(742, 509);
			this->splitContainer1->SplitterDistance = 590;
			this->splitContainer1->TabIndex = 0;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				58.99281F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				41.00719F)));
			this->tableLayoutPanel1->Controls->Add(this->gridColorButton, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->refreshButton, 0, 8);
			this->tableLayoutPanel1->Controls->Add(this->decayColorButton, 1, 5);
			this->tableLayoutPanel1->Controls->Add(this->decayPathCB, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->wallColorButton, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->pathColorButton, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->label3, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->fullPathCB, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->label4, 0, 5);
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->decaySpinner, 1, 6);
			this->tableLayoutPanel1->Controls->Add(this->label5, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->rotateCB, 0, 7);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 9;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 20)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(148, 509);
			this->tableLayoutPanel1->TabIndex = 2;
			// 
			// gridColorButton
			// 
			this->gridColorButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->gridColorButton->Location = System::Drawing::Point(90, 3);
			this->gridColorButton->Name = L"gridColorButton";
			this->gridColorButton->Size = System::Drawing::Size(55, 24);
			this->gridColorButton->TabIndex = 1;
			this->gridColorButton->UseVisualStyleBackColor = true;
			this->gridColorButton->Click += gcnew System::EventHandler(this, &MyForm::gridColor_clicked);
			// 
			// refreshButton
			// 
			this->refreshButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->SetColumnSpan(this->refreshButton, 2);
			this->refreshButton->Location = System::Drawing::Point(70, 483);
			this->refreshButton->Name = L"refreshButton";
			this->refreshButton->Size = System::Drawing::Size(75, 23);
			this->refreshButton->TabIndex = 3;
			this->refreshButton->Text = L"Refresh";
			this->refreshButton->UseVisualStyleBackColor = true;
			this->refreshButton->Click += gcnew System::EventHandler(this, &MyForm::RefreshMaze);
			// 
			// decayColorButton
			// 
			this->decayColorButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->decayColorButton->Location = System::Drawing::Point(90, 153);
			this->decayColorButton->Name = L"decayColorButton";
			this->decayColorButton->Size = System::Drawing::Size(55, 24);
			this->decayColorButton->TabIndex = 3;
			this->decayColorButton->UseVisualStyleBackColor = true;
			this->decayColorButton->Click += gcnew System::EventHandler(this, &MyForm::decayColor_clicked);
			// 
			// decayPathCB
			// 
			this->decayPathCB->AutoSize = true;
			this->decayPathCB->Checked = true;
			this->decayPathCB->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tableLayoutPanel1->SetColumnSpan(this->decayPathCB, 2);
			this->decayPathCB->Dock = System::Windows::Forms::DockStyle::Fill;
			this->decayPathCB->Location = System::Drawing::Point(3, 123);
			this->decayPathCB->Name = L"decayPathCB";
			this->decayPathCB->Size = System::Drawing::Size(142, 24);
			this->decayPathCB->TabIndex = 3;
			this->decayPathCB->Text = L"Decay Path Color";
			this->decayPathCB->UseVisualStyleBackColor = true;
			this->decayPathCB->CheckedChanged += gcnew System::EventHandler(this, &MyForm::decayCB_CheckedChanged);
			// 
			// wallColorButton
			// 
			this->wallColorButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->wallColorButton->Location = System::Drawing::Point(90, 33);
			this->wallColorButton->Name = L"wallColorButton";
			this->wallColorButton->Size = System::Drawing::Size(55, 24);
			this->wallColorButton->TabIndex = 1;
			this->wallColorButton->UseVisualStyleBackColor = true;
			this->wallColorButton->Click += gcnew System::EventHandler(this, &MyForm::wallColor_clicked);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->label2->Location = System::Drawing::Point(3, 30);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(81, 30);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Wall Color:";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// pathColorButton
			// 
			this->pathColorButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pathColorButton->Location = System::Drawing::Point(90, 63);
			this->pathColorButton->Name = L"pathColorButton";
			this->pathColorButton->Size = System::Drawing::Size(55, 24);
			this->pathColorButton->TabIndex = 1;
			this->pathColorButton->UseVisualStyleBackColor = true;
			this->pathColorButton->Click += gcnew System::EventHandler(this, &MyForm::pathColor_clicked);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->label3->Location = System::Drawing::Point(3, 60);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(81, 30);
			this->label3->TabIndex = 0;
			this->label3->Text = L"Path Color:";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// fullPathCB
			// 
			this->fullPathCB->AutoSize = true;
			this->fullPathCB->Checked = true;
			this->fullPathCB->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tableLayoutPanel1->SetColumnSpan(this->fullPathCB, 2);
			this->fullPathCB->Dock = System::Windows::Forms::DockStyle::Fill;
			this->fullPathCB->Location = System::Drawing::Point(3, 93);
			this->fullPathCB->Name = L"fullPathCB";
			this->fullPathCB->Size = System::Drawing::Size(142, 24);
			this->fullPathCB->TabIndex = 2;
			this->fullPathCB->Text = L"Show Full Path";
			this->fullPathCB->UseVisualStyleBackColor = true;
			this->fullPathCB->CheckedChanged += gcnew System::EventHandler(this, &MyForm::fullPathChecked);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Dock = System::Windows::Forms::DockStyle::Fill;
			this->label4->Location = System::Drawing::Point(3, 150);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(81, 30);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Decay Color:";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->label1->Location = System::Drawing::Point(3, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(81, 30);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Grid Color:";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// decaySpinner
			// 
			this->decaySpinner->Dock = System::Windows::Forms::DockStyle::Fill;
			this->decaySpinner->Location = System::Drawing::Point(90, 183);
			this->decaySpinner->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 60, 0, 0, 0 });
			this->decaySpinner->Name = L"decaySpinner";
			this->decaySpinner->Size = System::Drawing::Size(55, 20);
			this->decaySpinner->TabIndex = 5;
			this->decaySpinner->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->decaySpinner->ValueChanged += gcnew System::EventHandler(this, &MyForm::decayDelay_changed);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Dock = System::Windows::Forms::DockStyle::Fill;
			this->label5->Location = System::Drawing::Point(3, 180);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(81, 30);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Decay Delay:";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// rotateCB
			// 
			this->rotateCB->AutoSize = true;
			this->rotateCB->Checked = true;
			this->rotateCB->CheckState = System::Windows::Forms::CheckState::Checked;
			this->tableLayoutPanel1->SetColumnSpan(this->rotateCB, 2);
			this->rotateCB->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rotateCB->Location = System::Drawing::Point(3, 213);
			this->rotateCB->Name = L"rotateCB";
			this->rotateCB->Size = System::Drawing::Size(142, 24);
			this->rotateCB->TabIndex = 7;
			this->rotateCB->Text = L"Apply Rotation";
			this->rotateCB->UseVisualStyleBackColor = true;
			this->rotateCB->CheckedChanged += gcnew System::EventHandler(this, &MyForm::applyRot_changed);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(742, 509);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"MyForm";
			this->Text = L"MazeGen2";
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->decaySpinner))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		UNREFERENCED_PARAMETER(sender);
		UNREFERENCED_PARAMETER(e);
		glView->UpdateAnimation();
		//glView->Render();
		glView->SwapOpenGLBuffers();
	}

	

private: System::Void splitContainer1_Panel1_Resize(System::Object^  sender, System::EventArgs^  e) {
	System::Windows::Forms::Control^ container = (System::Windows::Forms::Control^)sender;
	glView->ReSizeGLScene(container->Width, container->Height);
}
private: System::Void RefreshMaze(System::Object^  sender, System::EventArgs^  e) {
	
	glView->RefreshWithBuilder();
}
private: System::Void fullPathChecked(System::Object^  sender, System::EventArgs^  e) {
	glView->drawPath = ((CheckBox^)sender)->Checked;
}

private: System::Void decayCB_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	glView->showDecayColor = ((CheckBox^)sender)->Checked;
}
private: System::Void gridColor_clicked(System::Object^  sender, System::EventArgs^  e) {
	ColorDialog^ dlg = gcnew ColorDialog();

	dlg->Color = glView->gridColor;
	if (dlg->ShowDialog() == ::DialogResult::OK)
	{
		glView->gridColor = dlg->Color;
		gridColorButton->BackColor = dlg->Color;
	}
}
private: System::Void wallColor_clicked(System::Object^  sender, System::EventArgs^  e) {

	ColorDialog^ dlg = gcnew ColorDialog();

	dlg->Color = glView->wallColor;
	if (dlg->ShowDialog() == ::DialogResult::OK)
	{
		glView->wallColor = dlg->Color;
		wallColorButton->BackColor = dlg->Color;
	}
}
private: System::Void pathColor_clicked(System::Object^  sender, System::EventArgs^  e) {

	ColorDialog^ dlg = gcnew ColorDialog();

	dlg->Color = glView->pathColor;
	if (dlg->ShowDialog() == ::DialogResult::OK)
	{
		glView->pathColor = dlg->Color;
		pathColorButton->BackColor = dlg->Color;
	}
}
private: System::Void decayColor_clicked(System::Object^  sender, System::EventArgs^  e) {
	ColorDialog^ dlg = gcnew ColorDialog();

	dlg->Color = glView->decayColor;
	if (dlg->ShowDialog() == ::DialogResult::OK)
	{
		glView->decayColor = dlg->Color;
		pathColorButton->BackColor = dlg->Color;
	}
}
private: System::Void decayDelay_changed(System::Object^  sender, System::EventArgs^  e) {
	
	glView->decayDelay = (int)((NumericUpDown^)sender)->Value;
}
private: System::Void applyRot_changed(System::Object^  sender, System::EventArgs^  e) {

	glView->doRotate = ((CheckBox^)sender)->Checked;
}
};
}
