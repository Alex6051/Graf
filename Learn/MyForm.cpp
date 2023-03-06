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

std::string getNode(std::string s)  //функця чтобы считать имя уровня из текста
{
    std::string get = "";
    for (int i = 0; i < s.size(); i++)  //идем по каждому символу строки
    {
        if (s[i] == '#') 
        {
            i += 2;                     //перескакиваем решетку в строке и табуляцию
            for (i; i < s.size(); i++)  //идем по каждому символу строки дальше
            {
                if (s[i] != '\t')       //проверка чтобы не было табуляции, чтобы считать всё до следующей табуляции
                {
                    get += s[i];        //записываем текст узла посимвольно
                }
                else { i = s.size(); }  //дошли до табуляции, значит тут делать больше нечего и заканчиваем цикл, можно было поставить break
            }
        }
    }
    return get;                             //возвращаем имя узла
}

void bigToSmall(String^ s, std::string& os) {    //копируем большой стринг в маленький
    using namespace Runtime::InteropServices;
    const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer(); //записываем String^ в chars
    os = chars;                                 //записываем chars в string os
    Marshal::FreeHGlobal(IntPtr((void*)chars));
}

std::string getParentNubmer(std::string s)   //получаем номер родителя узла
{
    int f;
    f = 0;
    std::string get = "";
    for (int i = 0; i < s.size(); i++)  //идем по каждому символу строки
    {
        if (s[i] == '#')
        {
            f += 1;         //отсчитываем какая по счету это решетка
            if (f == 2)     //если прошли уже 2 решетки
            {
                i += 2;     //перепрыгиваем решетку и табуляцию
                for (i; i < s.size(); i++)  //идем по каждому символу строки дальше
                {
                    get += s[i];            //записываем код родителя
                }
            }
        }
    }
    return get;                             //возвращаем код родителя
}

int number = 0;                     //номер узла
int parent = 0;                     //номер родителя
std::vector<std::string> masivNode; //массив с именами узлов

void WriteRecursive(StreamWriter^ writer, TreeNode^ treeNode) //запись уровней в файл
{
    number += 1;            //эта переменная нужна для нумерации уровней
    std::stringstream bufNumber;
    bufNumber << number;    //вводим  number в bufNumber, нужно чтобы записать цифры в stringstream, чтобы потом преобразовать в string
    std::string kod = bufNumber.str(); //преобразуем stringstream в string
    String^ Kod = gcnew String(kod.c_str()); //преобразуем string в String^  
    String^ nodeBuf = gcnew String("");     //просто буфер
    String^ nodeRod = gcnew String("");     
    std::string nodeRodstr = "";
    TreeNode^ node = gcnew TreeNode("");
    nodeBuf += treeNode->Text;              //заношу текст узла 
    std::string bufer;                      //просто буфер
    bigToSmall(nodeBuf, bufer);             //заношу в bufer значение nodeBuf, преобразую большой string в маленький
    masivNode.push_back(bufer);             //заношу имя узла в masivNode 
    if (treeNode->Parent != nullptr)        //если у узла есть родительский узел
    {
        nodeRod += treeNode->Parent->Text;  //заношу родительский узел
        bigToSmall(nodeRod, nodeRodstr);    //опять преобразуем, заносим текст родительского узла в nodeRodstr
        for (int schetWhile = 0; schetWhile < masivNode.size(); schetWhile++)
            if (masivNode[schetWhile] == nodeRodstr) parent = schetWhile + 1; //если имя masivNode = nodeRodstr, то заношу номер элемента массива в parent
    }
    std::stringstream bufParent;            //просто буфер 

    if (parent == 0 || treeNode->Parent == nullptr) // если нет родителей
    {
        bufParent << "";                    //типа обнулил
    }
    else { bufParent << parent; }           
    std::string parent_string = bufParent.str();    //заношу код родителя
    String^ Kodrod = gcnew String(parent_string.c_str());   //заношу код родителя
    String^ line = (Kod + "\t" + "#" + "\t" + treeNode->Text + "\t" + "#" + "\t" + Kodrod); //записываю номер узла, узел и код родителя
    writer->WriteLine(line);                //записываю в файл
    for each (TreeNode ^ tn in treeNode->Nodes) //идем по узлам
    {
        WriteRecursive(writer, tn);
    }
}

void CreateSaveFile(String^ fileName, TreeView^ treeView) //функция сохранения
{
    StreamWriter^ Writer = gcnew StreamWriter(fileName);
    TreeNodeCollection^ nodes = treeView->Nodes;        
    for each(TreeNode^ n in nodes)                      //проходимся по каждому уровню 
    {
        WriteRecursive(Writer, n);                      //передаем в функцию записи открытый файл и уровень для записи
    }
    Writer->Close();                                    //закрываем файл
    masivNode.clear();                                  //чистим массив от всех записанных уровней
    number = 0;                                         //обнуляем
}

System::Void Learn::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
    String^ Mesto;
    String^ line;
    String^ Read;
    int number,schetMasiv=0, schetMasivNode = 0;
    std::string kod, text, kodrod, stroka;
    std::vector<std::string> masivText;     //сюда буду заносить имена узлов и подузлов
    if (openFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        treeView1->Nodes->Clear();          //очищаем прошлые узлы, иначе они останутся и к ним добавятся новые в окне
        Mesto = openFileDialog1->FileName;  // запись пути к выбранному файлу
        label1->Text = Mesto;               //отображаем путь к файлу
        StreamReader^ file = gcnew StreamReader(openFileDialog1->FileName); //открывает файл
        TreeNode^ node = gcnew TreeNode("");
        Read = file->ReadLine();            //читаем строку из файла
        bigToSmall(Read, stroka);           //преобразуем ее в обычный string в переменную stroka
        text = getNode(stroka);             //получаем имя уровня
        line = gcnew String(text.c_str());  //преобразуем ее из string в String^ 
        node = treeView1->Nodes->Add(line); // добавляем новый узел и сразу его заносим в переменную node, чтобы потом мог на него ссылаться
        masivText.push_back(text); 
        array<TreeNode^>^ nodes = gcnew array<TreeNode^>(500); //массив всех узлов и подузлов
        nodes[schetMasiv] = node;           //добавляем первый узел в массив
        while ((Read = file->ReadLine()) != nullptr)  //цикл для чтения из файла 
        {
            schetMasiv += 1;
            bigToSmall(Read, stroka);
            if (getParentNubmer(stroka) == "") //если нет родителя у узла
            {
                kodrod = "0";
            }
            else {
                kodrod = getParentNubmer(stroka);
            }
            number = stoi(kodrod);          //преобразуем string в int
            text = getNode(stroka);
            masivText.push_back(text);
            line = gcnew String(text.c_str()); //преобразуем string в String^ чтобы смогли создать узел с таким именем
            if (number != 0)                //если нет родителя у узла
            {
                treeView1->SelectedNode = nodes[number-1]; //выделяем родительский узел
                nodes[schetMasiv] = treeView1->SelectedNode->Nodes->Add(line); //создаем подузел
            }
            else nodes[schetMasiv] = treeView1->Nodes->Add(line); //создаем узел
        }
        file->Close();
    }
    else
    {
        MessageBox::Show("Файл не был открыт", "Ошибка");
    }
    return System::Void();
}

System::Void Learn::MyForm::Save_Click(System::Object^ sender, System::EventArgs^ e) //кнопка сохранить 
{
    if (textBox1->Text != nullptr) //проверка, открывался ли какой-то файл
    {
        saveFileDialog1->ShowDialog();
        if (saveFileDialog1->FileName != "")
        {
            StreamWriter^ file = gcnew StreamWriter(saveFileDialog1->FileName); //сохранение файла
            String^ Mesto = saveFileDialog1->FileName; // запись пути к выбранному файлу
            label1->Text = Mesto; //отображаем путь к файлу в окне
            file->Close();
            CreateSaveFile(saveFileDialog1->FileName, treeView1);
        }
        else {                  //если никакой файл не открывался, значит создаем новый
            String^ Mesto = label1->Text; //берем путь к файлу из label1
            CreateSaveFile(Mesto, treeView1);
        }
    }
    return System::Void();
}

System::Void Learn::MyForm::Save_As_Click(System::Object^ sender, System::EventArgs^ e) //кнопка сохранить как
{
    saveFileDialog1->ShowDialog();
    if (saveFileDialog1->FileName != "")
    {
        StreamWriter^ file = gcnew StreamWriter(saveFileDialog1->FileName); //сохранение файла
        String^ Mesto = saveFileDialog1->FileName; // запись пути к выбранному файлу
        label1->Text = Mesto; //отображаем путь к файлу в окне
        file->Close();
        CreateSaveFile(saveFileDialog1->FileName, treeView1);
    }
    return System::Void();
}

System::Void Learn::MyForm::label1_Click(System::Object^ sender, System::EventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::button1_Click_1(System::Object^ sender, System::EventArgs^ e) //создать уровень
{
    TreeNode^ node = gcnew TreeNode(textBox1->Text); //получаем имя узла
    treeView1->Nodes->Add(node); //создаем узел
    return System::Void();
}

System::Void Learn::MyForm::button2_Click(System::Object^ sender, System::EventArgs^ e) //создать подуровень
{
    TreeNode^ node = gcnew TreeNode(comboBox1->Text); //получаем имя подузла
    if (node->Text != "" && treeView1->SelectedNode != nullptr)
    {
        treeView1->SelectedNode->Nodes->Add(node);//создаем подузел
    }
    else
    {
        MessageBox::Show("Выберите уровень и введите наименование подуровня", "Ошибка");
    }
    return System::Void();
}

System::Void Learn::MyForm::contextMenuStrip1_Opening(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::treeView1_AfterSelect(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e)
{
    change->Text = treeView1->SelectedNode->Text; //заносим в label change имя выделенного узла
    return System::Void();
}

System::Void Learn::MyForm::change_TextChanged(System::Object^ sender, System::EventArgs^ e)
{
    return System::Void();
}

System::Void Learn::MyForm::button3_Click(System::Object^ sender, System::EventArgs^ e)         //изменить уровень или подуровень
{
    treeView1->SelectedNode->Text = change->Text; //выделенный узел переименовываем, имя берем из label change
    return System::Void();
}

System::Void Learn::MyForm::button4_Click_1(System::Object^ sender, System::EventArgs^ e)        //удалить уровень или подуровень
{
    if (treeView1->SelectedNode != nullptr)
        treeView1->Nodes->Remove(treeView1->SelectedNode); 
    return System::Void();
}

System::Void Learn::MyForm::button4_Click(System::Object^ sender, System::EventArgs^ e) //закрываем программу
{
    exit(0);
}


