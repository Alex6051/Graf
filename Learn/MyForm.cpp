#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;

[STAThread]
void main(array<String^>^ arg) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Learn::MyForm form;
    Application::Run(% form);
}

System::Void Learn::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ FileName;
    if (openFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        FileName = openFileDialog1->FileName; // запись пути к выбранному файлу
    }
    try
    {
        StreamReader^ reader = File::OpenText(FileName); //открывает файл
    }
    catch (Exception^ e)
    {
        MessageBox::Show("Файл не был открыт", "Ошибка");
    }
    return System::Void();
}

System::Void Learn::MyForm::button2_Click(System::Object^ sender, System::EventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::button3_Click(System::Object^ sender, System::EventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::richTextBox1_TextChanged(System::Object^ sender, System::EventArgs^ e)
{
    
    return System::Void();
}


