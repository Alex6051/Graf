#include "MyForm.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

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

std::string getNode(std::string s)  //������ ����� ������� ��� ������ �� ������
{
    std::string get = "";
    for (int i = 0; i < s.size(); i++)  //���� �� ������� ������� ������
    {
        if (s[i] == '#') 
        {
            i += 2;                     //������������� ������� � ������ � ���������
            for (i; i < s.size(); i++)  //���� �� ������� ������� ������ ������
            {
                if (s[i] != '\t')       //�������� ����� �� ���� ���������, ����� ������� �� �� ��������� ���������
                {
                    get += s[i];        //���������� ����� ���� �����������
                }
                else { i = s.size(); }  //����� �� ���������, ������ ��� ������ ������ ������ � ����������� ����, ����� ���� ��������� break
            }
        }
    }
    return get;                             //���������� ��� ����
}

void bigToSmall(String^ s, std::string& os) {    //�������� ������� ������ � ���������
    using namespace Runtime::InteropServices;
    const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer(); //���������� String^ � chars
    os = chars;                                 //���������� chars � string os
    Marshal::FreeHGlobal(IntPtr((void*)chars));
}

std::string getParentNubmer(std::string s)   //�������� ����� �������� ����
{
    int f;
    f = 0;
    std::string get = "";
    for (int i = 0; i < s.size(); i++)  //���� �� ������� ������� ������
    {
        if (s[i] == '#')
        {
            f += 1;         //����������� ����� �� ����� ��� �������
            if (f == 2)     //���� ������ ��� 2 �������
            {
                i += 2;     //������������� ������� � ���������
                for (i; i < s.size(); i++)  //���� �� ������� ������� ������ ������
                {
                    get += s[i];            //���������� ��� ��������
                }
            }
        }
    }
    return get;                             //���������� ��� ��������
}

int number = 0;                     //����� ����
int parent = 0;                     //����� ��������
std::vector<std::string> masivNode; //������ � ������� �����

void WriteRecursive(StreamWriter^ writer, TreeNode^ treeNode) //������ ������� � ����
{
    number += 1;            //��� ���������� ����� ��� ��������� �������
    std::stringstream bufNumber;
    bufNumber << number;    //������  number � bufNumber, ����� ����� �������� ����� � stringstream, ����� ����� ������������� � string
    std::string kod = bufNumber.str(); //����������� stringstream � string
    String^ Kod = gcnew String(kod.c_str()); //����������� string � String^  
    String^ nodeBuf = gcnew String("");     //������ �����
    String^ nodeRod = gcnew String("");     
    std::string nodeRodstr = "";
    TreeNode^ node = gcnew TreeNode("");
    nodeBuf += treeNode->Text;              //������ ����� ���� 
    std::string bufer;                      //������ �����
    bigToSmall(nodeBuf, bufer);             //������ � bufer �������� nodeBuf, ���������� ������� string � ���������
    masivNode.push_back(bufer);             //������ ��� ���� � masivNode 
    if (treeNode->Parent != nullptr)        //���� � ���� ���� ������������ ����
    {
        nodeRod += treeNode->Parent->Text;  //������ ������������ ����
        bigToSmall(nodeRod, nodeRodstr);    //����� �����������, ������� ����� ������������� ���� � nodeRodstr
        for (int schetWhile = 0; schetWhile < masivNode.size(); schetWhile++)
            if (masivNode[schetWhile] == nodeRodstr) parent = schetWhile + 1; //���� ��� masivNode = nodeRodstr, �� ������ ����� �������� ������� � parent
    }
    std::stringstream bufParent;            //������ ����� 

    if (parent == 0 || treeNode->Parent == nullptr) // ���� ��� ���������
    {
        bufParent << "";                    //���� �������
    }
    else { bufParent << parent; }           
    std::string parent_string = bufParent.str();    //������ ��� ��������
    String^ Kodrod = gcnew String(parent_string.c_str());   //������ ��� ��������
    String^ line = (Kod + "\t" + "#" + "\t" + treeNode->Text + "\t" + "#" + "\t" + Kodrod); //��������� ����� ����, ���� � ��� ��������
    writer->WriteLine(line);                //��������� � ����
    for each (TreeNode ^ tn in treeNode->Nodes) //���� �� �����
    {
        WriteRecursive(writer, tn);
    }
}

void CreateSaveFile(String^ fileName, TreeView^ treeView) //������� ����������
{
    StreamWriter^ Writer = gcnew StreamWriter(fileName);
    TreeNodeCollection^ nodes = treeView->Nodes;        
    for each(TreeNode^ n in nodes)                      //���������� �� ������� ������ 
    {
        WriteRecursive(Writer, n);                      //�������� � ������� ������ �������� ���� � ������� ��� ������
    }
    Writer->Close();                                    //��������� ����
    masivNode.clear();                                  //������ ������ �� ���� ���������� �������
    number = 0;                                         //��������
}

System::Void Learn::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ Mesto;
    String^ line;
    String^ Read;
    int number,schetMasiv=0, schetMasivNode = 0;
    std::string kod, text, kodrod, stroka;
    std::vector<std::string> masivText;     //���� ���� �������� ����� ����� � ��������
    if (openFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        treeView1->Nodes->Clear();          //������� ������� ����, ����� ��� ��������� � � ��� ��������� ����� � ����
        Mesto = openFileDialog1->FileName;  // ������ ���� � ���������� �����
        label1->Text = Mesto;               //���������� ���� � �����
        StreamReader^ file = gcnew StreamReader(openFileDialog1->FileName); //��������� ����
        TreeNode^ node = gcnew TreeNode("");
        Read = file->ReadLine();            //������ ������ �� �����
        bigToSmall(Read, stroka);           //����������� �� � ������� string � ���������� stroka
        text = getNode(stroka);             //�������� ��� ������
        line = gcnew String(text.c_str());  //����������� �� �� string � String^ 
        node = treeView1->Nodes->Add(line); // ��������� ����� ���� � ����� ��� ������� � ���������� node, ����� ����� ��� �� ���� ���������
        masivText.push_back(text); 
        array<TreeNode^>^ nodes = gcnew array<TreeNode^>(500); //������ ���� ����� � ��������
        nodes[schetMasiv] = node;           //��������� ������ ���� � ������
        while ((Read = file->ReadLine()) != nullptr)  //���� ��� ������ �� ����� 
        {
            schetMasiv += 1;
            bigToSmall(Read, stroka);
            if (getParentNubmer(stroka) == "") //���� ��� �������� � ����
            {
                kodrod = "0";
            }
            else {
                kodrod = getParentNubmer(stroka);
            }
            number = stoi(kodrod);          //����������� string � int
            text = getNode(stroka);
            masivText.push_back(text);
            line = gcnew String(text.c_str()); //����������� string � String^ ����� ������ ������� ���� � ����� ������
            if (number != 0)                //���� ��� �������� � ����
            {
                treeView1->SelectedNode = nodes[number-1]; //�������� ������������ ����
                nodes[schetMasiv] = treeView1->SelectedNode->Nodes->Add(line); //������� �������
            }
            else nodes[schetMasiv] = treeView1->Nodes->Add(line); //������� ����
        }
        file->Close();
    }
    else
    {
        MessageBox::Show("���� �� ��� ������", "������");
    }
    return System::Void();
}

System::Void Learn::MyForm::Save_Click(System::Object^ sender, System::EventArgs^ e) //������ ��������� 
{
    if (textBox1->Text != nullptr) //��������, ���������� �� �����-�� ����
    {
        saveFileDialog1->ShowDialog();
        if (saveFileDialog1->FileName != "")
        {
            StreamWriter^ file = gcnew StreamWriter(saveFileDialog1->FileName); //���������� �����
            String^ Mesto = saveFileDialog1->FileName; // ������ ���� � ���������� �����
            label1->Text = Mesto; //���������� ���� � ����� � ����
            file->Close();
            CreateSaveFile(saveFileDialog1->FileName, treeView1);
        }
        else {                  //���� ������� ���� �� ����������, ������ ������� �����
            String^ Mesto = label1->Text; //����� ���� � ����� �� label1
            CreateSaveFile(Mesto, treeView1);
        }
    }
    return System::Void();
}

System::Void Learn::MyForm::Save_As_Click(System::Object^ sender, System::EventArgs^ e) //������ ��������� ���
{
    saveFileDialog1->ShowDialog();
    if (saveFileDialog1->FileName != "")
    {
        StreamWriter^ file = gcnew StreamWriter(saveFileDialog1->FileName); //���������� �����
        String^ Mesto = saveFileDialog1->FileName; // ������ ���� � ���������� �����
        label1->Text = Mesto; //���������� ���� � ����� � ����
        file->Close();
        CreateSaveFile(saveFileDialog1->FileName, treeView1);
    }
    return System::Void();
}

System::Void Learn::MyForm::label1_Click(System::Object^ sender, System::EventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::button1_Click_1(System::Object^ sender, System::EventArgs^ e) //������� �������
{
    TreeNode^ node = gcnew TreeNode(textBox1->Text); //�������� ��� ����
    treeView1->Nodes->Add(node); //������� ����
    return System::Void();
}

System::Void Learn::MyForm::button2_Click(System::Object^ sender, System::EventArgs^ e) //������� ����������
{
    TreeNode^ node = gcnew TreeNode(comboBox1->Text); //�������� ��� �������
    if (node->Text != "" && treeView1->SelectedNode != nullptr)
    {
        treeView1->SelectedNode->Nodes->Add(node);//������� �������
    }
    else
    {
        MessageBox::Show("�������� ������� � ������� ������������ ���������", "������");
    }
    return System::Void();
}

System::Void Learn::MyForm::contextMenuStrip1_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::treeView1_AfterSelect(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e)
{
    change->Text = treeView1->SelectedNode->Text; //������� � label change ��� ����������� ����
    return System::Void();
}

System::Void Learn::MyForm::change_TextChanged(System::Object^ sender, System::EventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::button3_Click(System::Object^ sender, System::EventArgs^ e)         //�������� ������� ��� ����������
{
    treeView1->SelectedNode->Text = change->Text; //���������� ���� ���������������, ��� ����� �� label change
    return System::Void();
}

System::Void Learn::MyForm::button4_Click_1(System::Object^ sender, System::EventArgs^ e)        //������� ������� ��� ����������
{
    if (treeView1->SelectedNode != nullptr)
        treeView1->Nodes->Remove(treeView1->SelectedNode); 
    return System::Void();
}

System::Void Learn::MyForm::button4_Click(System::Object^ sender, System::EventArgs^ e) //��������� ���������
{
    exit(0);
}


