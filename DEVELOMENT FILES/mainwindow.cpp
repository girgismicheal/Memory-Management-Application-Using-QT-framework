#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <QPainter>
#include <QGraphicsItem>
#include <QMessageBox>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene =new QGraphicsScene (this);
    ui->graphicsView->setScene(scene);
    ui->treeWidget->setColumnCount(3);
    ui->treeWidget->setHeaderLabels(QStringList()<<"seg"<<"base"<<"size");

}
MainWindow::~MainWindow()
{
    delete ui;
}

int mem_size;//memory size variabel
vector<block>free1;//holes table sorted here
vector<block>alloc;//allocated tabel sorted here
bool compareByStart(const block &a, const block &b)//a function for purpse of sorting
{
    return a.start < b.start;
}
bool compareBySize(const block &a, const block &b)//a function for purpse of sorting
{
    return a.size < b.size;
}
class proj{
public:
    string name;
    int num_seg;
    int allocated;
    vector <block>segs;
    proj(){num_seg=0;allocated=0;}// at first num seg =0 and not allocated
//    void insert_seg(){
//        for(int i=0;i<num_seg;i++)
//        {
//            block in;
//            cin>>in.name>>in.size;
//            segs.push_back(in);

//        }
//    }
//    void print_seg(){
//        for(int i=0;i<segs.size();i++)
//        {
//            cout<<segs[i].name<<" "<<segs[i].start<<" "<<segs[i].size<<" "<<segs[i].end<<endl;
//        }
//    }
    void first_fit(){//algorithms of first fit
        /*we asume we have the holes and allocated tabel and the process with it's
         1-name 2-num seg 3-seg data
         the we are going to use the algorithm so allocate this process in the holes
         if that can happen
        */
        int counter=0;// a counter for counting the number of the seg could be allocate
        vector <block>first_vec=free1;//copy the table of holes to change in
        vector <block>first_vec_alloc=alloc;//copy the allocation rable
        for(int n=0;n<num_seg;n++){// allocate each segment

            //sort(first_vec.begin(),first_vec.end(),compareByStart);//sort them before the new allocation
            vector<block>::iterator it =first_vec.begin();
            for(int i =0;i<first_vec.size();i++){//try finding the suitable hole for allocation
                if(first_vec[i].size>=segs[n].size){//size of hole bigger or equel the size of seg then allocate it

                    segs[n].start=first_vec[i].start;
                    first_vec[i].size-=segs[n].size;
                    first_vec[i].start+=segs[n].size;
                    segs[n].end=segs[n].start+segs[n].size-1;
                    counter++;
                    if(first_vec[i].size<1){first_vec.erase(it);}
                    break;
                }
                it++;

            }
        }
        if(counter==num_seg &&!allocated){//if all the seg can be allocated and the process is not allocated the  change the table and allocate it
                allocated=1;
                free1=first_vec;
                for(int n=0;n<num_seg;n++){
                    alloc.push_back(segs[n]);
                }
        }
    }
    void best_fit(){
        int counter=0;
        vector <block>first_vec=free1;
        vector <block>first_vec_alloc=alloc;
        sort(first_vec.begin(),first_vec.end(),compareBySize);
        //for(int n=0;n<first_vec.size();n++){cout<<first_vec[n].start<<"  ";}
        //cout<<endl;
        for(int n=0;n<num_seg;n++){
            vector<block>::iterator it =first_vec.begin();
            for(int i =0;i<first_vec.size();i++){

                if(first_vec[i].size>=segs[n].size){

                    segs[n].start=first_vec[i].start;
                    first_vec[i].size-=segs[n].size;
                    first_vec[i].start+=segs[n].size;
                    segs[n].end=segs[n].start+segs[n].size-1;
                    if(first_vec[i].size<1){first_vec.erase(it);}
                    counter++;
                    sort(first_vec.begin(),first_vec.end(),compareBySize); 
                    break;
                }
                it++;
            }

        }
         if(counter==num_seg&&!allocated){
                allocated=1;
                cout<<"hello"<<counter<<endl;
                free1=first_vec;
                for(int n=0;n<num_seg;n++){
                    alloc.push_back(segs[n]);
                }
        }
    }
};
vector <proj>proces;
//void add_process(){
//        proj new_proccess;
//        cin>>new_proccess.name>>new_proccess.num_seg;
//        new_proccess.insert_seg();
//        string s;cin>>s;
//        if(s=="first"){
//            new_proccess.first_fit();
//        }
//        else if(s=="best"){
//            new_proccess.best_fit();
//        }
//        new_proccess.print_seg();
//        proces.push_back(new_proccess);
//}

void de_allocate(block seg){

    vector<block>::iterator it =alloc.begin();// we catch the first block
    sort(alloc.begin(),alloc.end(),compareByStart);//sort both tables by start
    sort(free1.begin(),free1.end(),compareByStart);

    for(int i =0;i<alloc.size();i++){//search for the one that must be allocated

            if (seg.start ==alloc[i].start){

                cout<<"yes i am"<<seg.name<<"go to delete"<<seg.start<<" "<<seg.size<<" "<<seg.end<<endl;
                //all the memory
                if(seg.size==mem_size){
                    block x=seg;
                    free1.push_back(x);
                    alloc.erase(it);
                    break;
            }
            //start
            else if(seg.start==0){

                if(seg.end+1!=alloc[i+1].start){// start after me free
                    cout<<"i am start hole";
                    free1[0].start=0;
                    free1[0].size= free1[0].end- free1[0].start+1;
                    alloc.erase(it);
                    break;
                }
                else if(seg.end+1==alloc[i+1].start ){//start after me allocated
                    cout<<"i am start alloc";
                    block x=seg;
                    free1.push_back(x);
                    alloc.erase(it);
                    break;
                }
            }
            //end
            else if(seg.end==mem_size-1){

                if(seg.start-1!=alloc[i-1].end){//hole
                    cout<<"i am end hole";
                    free1.back().end=mem_size-1;
                    free1.back().size= free1.back().end- free1.back().start+1;
                    alloc.erase(it);
                    break;
                }
                else if(seg.start-1==alloc[i-1].end){//alloc
                    cout<<"i am start alloc";
                    block x=seg;
                    free1.push_back(x);
                    alloc.erase(it);
                    break;
                }
            }
            //end
            //alloc alloc
            else if (seg.start-1== alloc[i-1].end&&seg.end+1==alloc[i+1].start){
                cout<<"i am alloc alloc";
                block x;x.start=seg.start;x.end=seg.end;x.size=seg.size;
                free1.push_back(x);
                alloc.erase(it);
                break;
            }
            //hole alloc
            else if (seg.start-1!= alloc[i-1].end&&seg.end+1==alloc[i+1].start){
                cout<<"i am hole alloc";
                for(int i =0;i<free1.size();i++){
                    if(seg.start-1==free1[i].end){
                        free1[i].end=seg.end;
                        free1[i].size=free1[i].end-free1[i].start+1;
                        alloc.erase(it);
                        break;
                    }
                }
            }
            //alloc hole
            else if (seg.start-1== alloc[i-1].end&&seg.end+1!=alloc[i+1].start){
                cout<<"i am alloc hole";
                for(int i =0;i<free1.size();i++){
                    if(seg.end+1==free1[i].start){
                        free1[i].start-=seg.size;
                        free1[i].size=free1[i].end-free1[i].start+1;
                        alloc.erase(it);
                        break;
                    }
                }
            }
            //hole hole
            else if (seg.start-1!= alloc[i-1].end&&seg.end+1!=alloc[i+1].start){
                vector<block>::iterator it2 =free1.begin();
                cout<<"i am hole hole";
                int before,after;
                for(int i =0;i<free1.size();i++){
                        if(seg.start-1==free1[i].end)
                        {before=i;break;}
                }
                 for(int i =0;i<free1.size();i++){
                    if(seg.end+1==free1[i].start){
                        after=i;break;
                    }
                }
                for(int blela=0;blela<after;blela++){it2++;}
                free1[before].size+=(free1[after].size+seg.size);
                free1[before].end=free1[after].end;
                free1.erase(it2);
                alloc.erase(it);
            }
            else{ cout<<"i am nothing";}
            break;
        }
        it++;
    }

}
void de_allocate_process(proj p){// deallocate all the process segments
    /*for deallocate a process
        we would deallocate the segements first
    */
    for(int i=0;i<p.num_seg;i++){
        //cout<<p.segs[i].name<<"try to deleted ";
        sort(free1.begin(), free1.end(),compareByStart);
        sort(alloc.begin(),alloc.end(),compareByStart);
        de_allocate(p.segs[i]);
        //cout<<p.segs[i].name<<"deleted "<<endl;
    }

}

//memory entering and reset
void MainWindow::on_lineEdit_memSize_editingFinished()
{
    scene->clear();
    //frist we clear all vectors and tree widget
    alloc.clear();
    free1.clear();
    proces.clear();
    ui->treeWidget->clear();
    //take the size from line edit then generate the block of memory
    mem_size= ui->lineEdit_memSize->text().toInt();
    block neo;
    neo.start=0;
    neo.size=mem_size;
    neo.end=mem_size-1;
    neo.name="alloc for system";
    alloc.push_back(neo);
    squer =new MySquer(neo,false);
    scene->addItem(squer);
    //draw
    QString strInfo = QString::fromStdString(neo.name)+" "+QString::number(neo.start)+" "+QString::number(neo.end);
    scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,neo.start));
}

//add hole button
void MainWindow::on_pushButton_Addhole_clicked()
{
    //
    scene->clear();
    //add hole button
    int start,size0;
    start=ui->lineEdit_holestart->text().toInt();
    size0=ui->lineEdit_holeSize->text().toInt();
    //block allocation
    block x;
    x.name="hole";
    x.start=start;//last
    x.size=size0;
    x.end=start+size0-1;//last edit

    sort(alloc.begin(),alloc.end(),compareByStart);
    sort(free1.begin(),free1.end(),compareByStart);
    //logic of holes allocation
    if(free1.empty()){
        if(start==0){
            free1.push_back(x);
            alloc.back().start=x.end+1;
            alloc.back().size=alloc.back().end-alloc.back().start+1;
        }
        else{
            block alloc_new;
            alloc_new.name="alloc for system";
            alloc_new.start=alloc.back().start;
            alloc_new.end=start-1;
            alloc_new.size=alloc_new.end-alloc_new.start+1;
            alloc.back().start=x.end+1;
            alloc.back().size=alloc.back().end-alloc.back().start+1;
            alloc.push_back(alloc_new);
            free1.push_back(x);
        }
    }
    else{
        if(free1.back().end+1==start){
            free1.back().end=x.end;
            free1.back().size=free1.back().end-free1.back().start+1;
            alloc.back().start=x.end+1;
            alloc.back().size=alloc.back().end-alloc.back().start+1;
        }
        else{
            block alloc_new;
            alloc_new.name="alloc for system";
            alloc_new.start=alloc.back().start;
            alloc_new.end=start-1;
            alloc_new.size=alloc_new.end-alloc_new.start+1;
            alloc.back().start=x.end+1;
            alloc.back().size=alloc.back().end-alloc.back().start+1;
            alloc.push_back(alloc_new);
            free1.push_back(x);
        }
    }

    sort(alloc.begin(),alloc.end(),compareByStart);
    sort(free1.begin(),free1.end(),compareByStart);
    //draw holes
    for(int i=0;i<free1.size();i++){
        QString strInfo = QString::fromStdString(free1[i].name)+" "+QString::number(free1[i].start)+" "+QString::number(free1[i].end) +" "+QString::number(free1[i].size);
        scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(200,free1[i].start));
        squer =new MySquer(free1[i],true);
        scene->addItem(squer);
    }
    //draw alloc
    for(int i=0;i<alloc.size();i++){
        QString strInfo = QString::fromStdString(alloc[i].name)+" "+QString::number(alloc[i].start)+" "+QString::number(alloc[i].end)+" "+QString::number(alloc[i].size);
        scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,alloc[i].start));
        squer =new MySquer(alloc[i],false);
        scene->addItem(squer);
    }
}
//reomove form system by dealloc button by start
void MainWindow::on_pushButton_4_clicked()
{
        scene->clear();
        //remove alloc from system
        int pos=ui->lineEdit_dealloc->text().toInt();//take the start from the box and find it in the allocation table
        for(int i =0;i<alloc.size();i++){
            if(pos==alloc[i].start && alloc[i].name=="alloc for system"){
                de_allocate(alloc[i]);
            }
            else if (pos==alloc[i].start){
                QMessageBox::information(this,"info","this part for a process can't be free");
            }
        }

        //draw holes
        for(int i=0;i<free1.size();i++){
            QString strInfo = QString::fromStdString(free1[i].name)+" "+QString::number(free1[i].start)+" "+QString::number(free1[i].end)+" "+QString::number(free1[i].size);
            scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,free1[i].start));
            squer =new MySquer(free1[i],true);
            scene->addItem(squer);
        }
        //draw alloc
        for(int i=0;i<alloc.size();i++){
            if(alloc[i].name=="alloc for system"){
                QString strInfo = QString::fromStdString(alloc[i].name)+" "+QString::number(alloc[i].start)+" "+QString::number(alloc[i].end)+" "+QString::number(alloc[i].size);
                scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,alloc[i].start));
                squer =new MySquer(alloc[i],false);
                scene->addItem(squer);
            }
        }
        //draw allocated porcess
        if(!proces.empty()){
            for(int i=0;i<proces.size();i++){
                if(proces[i].allocated){

                    for(int j=0;j<proces[i].segs.size();j++){
                        QString strInfo =QString::fromStdString(proces[i].name)+"has seg "+ QString::fromStdString(proces[i].segs[j].name)+" from "+QString::number(proces[i].segs[j].start)+" to "+QString::number(proces[i].segs[j].end)+" "+QString::number(proces[i].segs[j].size);
                        scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,proces[i].segs[j].start));
                        squer =new MySquer(proces[i].segs[j],false);
                        scene->addItem(squer);
                    }
                }
            }
        }
}
//just add process name to the compo
//this button doesnt need to draw any thing after pressing it.
void MainWindow::on_pushButton_addPocess_clicked()
{
    //add to the compo
    // if the comboe is empty the ask the user to fill it and try again
    if(ui->lineEdit_processName->text().isEmpty() ||ui->lineEdit_Numseg->text().isEmpty()){
        QMessageBox::warning(this,"","make sure you entered process name and segements number");
    }
    //else add the process to the process vector
    // using this button allow us to add the process name and it is segments number
    else{
        //make each process has a uniqe name
        for (int i=0;i<proces.size();i++){
            if(proces[i].name==ui->lineEdit_processName->text().toStdString()){
                QMessageBox::warning(this,"warning","this process is allready exist");
                return;
            }
        }


        QString process=ui->lineEdit_processName->text();
        //add it to the combo box of the process
        ui->comboBox_process->addItem(process);
        // add the process to the process vector
        proj newpor;
        newpor.num_seg=ui->lineEdit_Numseg->text().toInt();
        newpor.name=process.toStdString();
        proces.push_back(newpor);
    }
}
//add segs ot the process and this button doesnot need any drawing
void MainWindow::on_pushButton_5_clicked()
{
    //get the process to add the segement to
    QString process=ui->comboBox_process->currentText();
    for(int i =0;i<proces.size();i++){//search for the process
        if(proces[i].name==process.toStdString()){//if found
            if(proces[i].num_seg>=proces[i].segs.size()+1){//if it's size less than the process seg number add else tell the user
                // make new block take the size and name then push it to the process
                block new_seg;
                new_seg.name=ui->lineEdit_Segname->text().toStdString();
                new_seg.size=ui->lineEdit_Segsize->text().toInt();
                proces[i].segs.push_back(new_seg);
            }
            else{QMessageBox::information(this,"","this process has the required number of segments");break;}// last edit
        }
    }
}
//allocate the process and need drawing
void MainWindow::on_pushButton_3_clicked()
{
    //alloc a process
    scene->clear();
    //take the process name and the algorithm to allocate by
    QString process=ui->comboBox_process->currentText();
    QString op=ui->comboBox_operation->currentText();
    // find the process
    for(int i =0;i<proces.size();i++){
        if(proces[i].name==process.toStdString()){
            if(proces[i].num_seg>proces[i].segs.size()){
                QMessageBox::information(this,"error","this process have only "+QString::number(proces[i].segs.size())+"and it should have "+ QString::number(proces[i].num_seg));
                break;
            }
            //applay the algorithm
            if(op=="first"){
                proces[i].first_fit();

            }
            else if(op=="best"){
                proces[i].best_fit();
            }

            // if allocated successfully add it to the tree widget
            if( proces[i].allocated){

                QString removeitem=QString::fromStdString(proces[i].name);
                ui->comboBox_allocated_porcess->addItem(ui->comboBox_process->currentText());
                ui->comboBox_process->removeItem(ui->comboBox_process->currentIndex());
                //test
                QTreeWidgetItem *itm=new QTreeWidgetItem(ui->treeWidget);
                itm->setText(0,QString::fromStdString(proces[i].name));
                ui->treeWidget->addTopLevelItem(itm);
                for(int j=0;j<proces[i].segs.size();j++){
                    QTreeWidgetItem *itm2=new QTreeWidgetItem();
                    itm2->setText(0,QString::fromStdString(proces[i].segs[j].name));
                    itm2->setText(1,QString::number(proces[i].segs[j].start));
                    itm2->setText(2,QString::number(proces[i].segs[j].size));
                    itm->addChild(itm2);
                    ui->treeWidget->removeItemWidget(itm,0);
                }

            }
            else{QMessageBox::information(this,"notification","this process cant be allocated deallocate and try again");}
            break;
        }
    }


    //drawing the memory
    sort(alloc.begin(),alloc.end(),compareByStart);
    sort(free1.begin(),free1.end(),compareByStart);
    //draw holes

    for(int i=0;i<free1.size();i++){
        //if(free1[i].size<1) { continue;}
        QString strInfo = QString::fromStdString(free1[i].name)+" "+QString::number(free1[i].start)+" "+QString::number(free1[i].end)+" "+QString::number(free1[i].size);
        scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,free1[i].start));
        squer =new MySquer(free1[i],true);
        scene->addItem(squer);

    }
    //draw alloc
    for(int i=0;i<alloc.size();i++){
        //if(alloc[i].size<1){continue;}
        if(alloc[i].name=="alloc for system"){
            QString strInfo = QString::fromStdString(alloc[i].name)+" "+QString::number(alloc[i].start)+" "+QString::number(alloc[i].end)+" "+QString::number(alloc[i].size);
            scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,alloc[i].start));
            squer =new MySquer(alloc[i],false);
            scene->addItem(squer);
        }
    }
    //draw allocated porcess
    if(!proces.empty()){
        for(int i=0;i<proces.size();i++){
            if(proces[i].allocated){

                for(int j=0;j<proces[i].segs.size();j++){
                    QString strInfo =QString::fromStdString(proces[i].name)+"has seg "+ QString::fromStdString(proces[i].segs[j].name)+" from "+QString::number(proces[i].segs[j].start)+" to "+QString::number(proces[i].segs[j].end)+" "+QString::number(proces[i].segs[j].size);
                    scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,proces[i].segs[j].start));
                    squer =new MySquer(proces[i].segs[j],false);
                    scene->addItem(squer);
                }
            }
        }
    }


}
//deallocate the whole porcess and this buttom need drawing
void MainWindow::on_pushButton_dealloc_process_clicked()
{
    scene->clear();
    /*
     first we find the process in the process table
     and dealocate it using dealocate_process function then delete the function from the process vector
     and remove the process name from the allocated_combo_box
    */
    vector<proj>::iterator it =proces.begin();
    for(int i=0;i<proces.size();i++){
        if(proces[i].name ==ui->comboBox_allocated_porcess->currentText().toStdString() ){
            de_allocate_process(proces[i]);
            proces.erase(it);
            break;
        }
        it++;
    }
    ui->comboBox_allocated_porcess->removeItem(ui->comboBox_allocated_porcess->currentIndex());
    //draw holes
    for(int i=0;i<free1.size();i++){
        //if(free1[i].size<1){continue;}
        QString strInfo = QString::fromStdString(free1[i].name)+" "+QString::number(free1[i].start)+" "+QString::number(free1[i].end)+" "+QString::number(free1[i].size);
        scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,free1[i].start));
        squer =new MySquer(free1[i],true);
        scene->addItem(squer);
    }
    //draw alloc
    for(int i=0;i<alloc.size();i++){
        //if(alloc[i].size<1){continue;}
        if(alloc[i].name=="alloc for system"){
            QString strInfo = QString::fromStdString(alloc[i].name)+" "+QString::number(alloc[i].start)+" "+QString::number(alloc[i].end)+" "+QString::number(alloc[i].size);
            scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,alloc[i].start));
            squer =new MySquer(alloc[i],false);
            scene->addItem(squer);
        }
    }
    //draw allocated porcess
    if(!proces.empty()){
        for(int i=0;i<proces.size();i++){
            if(proces[i].allocated){

                for(int j=0;j<proces[i].segs.size();j++){
                    QString strInfo =QString::fromStdString(proces[i].name)+"has seg "+ QString::fromStdString(proces[i].segs[j].name)+" from "+QString::number(proces[i].segs[j].start)+" to "+QString::number(proces[i].segs[j].end)+" "+QString::number(proces[i].segs[j].size);
                    scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,proces[i].segs[j].start));
                    squer =new MySquer(proces[i].segs[j],false);
                    scene->addItem(squer);
                }
            }
        }
    }

//    //drawing the memory
//    sort(alloc.begin(),alloc.end(),compareByStart);
//    sort(free1.begin(),free1.end(),compareByStart);
//    //draw holes
//    for(int i=0;i<free1.size();i++){
//        QString strInfo = QString::fromStdString(free1[i].name)+" "+QString::number(free1[i].start)+" "+QString::number(free1[i].end);
//        scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,free1[i].start));
//        squer =new MySquer(free1[i],true);
//        scene->addItem(squer);
//    }
//    //draw alloc
//    for(int i=0;i<alloc.size();i++){
//        if(alloc[i].name=="alloc for system"){
//            QString strInfo = QString::fromStdString(alloc[i].name)+" "+QString::number(alloc[i].start)+" "+QString::number(alloc[i].end);
//            scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,alloc[i].start));
//            squer =new MySquer(alloc[i],false);
//            scene->addItem(squer);
//        }
//    }
//    //draw allocated porcess
//    if(!proces.empty()){
//        for(int i=0;i<proces.size();i++){
//            if(proces[i].allocated){

//                for(int j=0;j<proces[i].segs.size();j++){
//                    QString strInfo =QString::fromStdString(proces[i].name)+"has seg "+ QString::fromStdString(proces[i].segs[j].name)+" from "+QString::number(proces[i].segs[j].start)+" to "+QString::number(proces[i].segs[j].end);
//                    scene->addText(strInfo,QFont("Segoe UI", 10, QFont::Bold))->setPos(QPoint(100,proces[i].segs[j].start));
//                    squer =new MySquer(proces[i].segs[j],false);
//                    scene->addItem(squer);
//                }
//            }
//        }
//    }
}

//deallocate whole process form the tree
void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    //QString op= ui->treeWidget->currentItem()->text(0);
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"","would you want to deallocate this process form the memory!!",QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes){
            delete ui->treeWidget->currentItem();
    }
    else{
        vector<block>::iterator it =free1.begin();
        for (int i=0;i<free1.size();i++) {
            if(free1[i].size<1){free1.erase(it);}
            it++;
        }
        vector<block>::iterator it1 =free1.begin();
        for (int i=0;i<alloc.size();i++) {
            if(alloc[i].size<1){alloc.erase(it1);}
            it++;
        }
    }
}



