#include "listaddingdialog.h"
#include "ui_listaddingdialog.h"

ListAddingDialog::ListAddingDialog(QWidget *parent, QString listName) :
    QDialog(parent),
    ui(new Ui::ListAddingDialog)
{
    ui->setupUi(this);
    ui->listNameInput_label->setText(listName);
    ui->priceInput->setText(QString::number(priceVal));
    ui->priceInput->setReadOnly(true);

    // Connect number 0-9 and math operations ui with the function
    connect(ui->button0,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button1,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button2,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button3,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button4,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button5,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button6,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button7,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button8,SIGNAL(clicked()), this, SLOT(numButton_clicked()));
    connect(ui->button9,SIGNAL(clicked()), this, SLOT(numButton_clicked()));

    connect(ui->divide_button, SIGNAL(clicked()), this, SLOT(mathButton_clicked()));
    connect(ui->multiply_button, SIGNAL(clicked()), this, SLOT(mathButton_clicked()));
    connect(ui->minus_button, SIGNAL(clicked()), this, SLOT(mathButton_clicked()));
    connect(ui->plus_button, SIGNAL(clicked()), this, SLOT(mathButton_clicked()));
}

ListAddingDialog::~ListAddingDialog()
{
    delete ui;
}

// Get price value
int ListAddingDialog::getPriceVal()
{
    return ui->priceInput->text().toInt();
}

// Assign each payer's name into checkable button
void ListAddingDialog::setNameList()
{
    nameGridLayout = new QGridLayout(ui->nameList_scrollAreaWidgetContents);
    for (int i{0}; i < payerList.size(); i++)
    {
        // Get payers' name
        QPushButton *nameButton = new QPushButton(payerList.at(i)->getName(), this);
        nameButton->setStyleSheet("QPushButton{font: 14px;font: bold; color: #0f4c75; border: solid; border-radius: 6px; border-color: #0f4c75; border-width: 2px;}"
                                  "QPushButton:checked{background-color: #0f4c75; color: #f6f5f5; border: outset; border-radius: 6px;}");
        nameButton->setFixedSize(QSize(280,30));
        nameButton->setCheckable(true);
        nameGridLayout->setSpacing(1);

        // Add all names into widget and update data
        nameGridLayout->addWidget(nameButton,i,0,Qt::AlignCenter);
        nameButtonList.push_back(nameButton);
    }
}

// Update total price
void ListAddingDialog::setNewTotal(int newTotal)
{
    int oldTotal = total->text().toInt();
    total->setText(QString::number(oldTotal + newTotal));
}

// Set payer list into the dialog
void ListAddingDialog::setPayerList(QVector<Payer *> list)
{
    payerList = list;
    setNameList();
}

// Set list pointer
void ListAddingDialog::setItem(List *item)
{
    this->itemPtr = item;
}

// Set total pointer
void ListAddingDialog::setTotal(QLabel *total)
{
    this->total = total;
}

// Display number 0-9
void ListAddingDialog::numButton_clicked()
{
    QPushButton *button = (QPushButton *) sender();
    QString buttonVal = button->text();
    QString getPriceVal = ui->priceInput->text();

    if(getPriceVal.toInt() == 0)
    {
        ui->priceInput->setText(buttonVal);
    }
    else
    {
        QString enterNum = getPriceVal + buttonVal;
        int currentPrice = enterNum.toInt();
        ui->priceInput->setText(QString::number(currentPrice));
    }
}

// Do different math operations (+,-,*,/)
void ListAddingDialog::mathButton_clicked()
{
    QString getPriceVal = ui->priceInput->text();
    priceVal = getPriceVal.toInt();
    QPushButton *button = (QPushButton *) sender();
    QString butVal = button->text();
    if(QString::compare(butVal, "÷", Qt::CaseInsensitive) == 0)
    {
        isDivide = true;
    }
    else if(QString::compare(butVal, "×", Qt::CaseInsensitive) == 0)
    {
        isMultiply = true;
    }
    else if(QString::compare(butVal, "−", Qt::CaseInsensitive) == 0)
    {
        isSubtract = true;
    }
    else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0)
    {
        isAdd = true;
    }
    ui->priceInput->setText("");
}

// Show result after calculation
void ListAddingDialog::on_equal_button_clicked()
{
    QString getPriceVal = ui->priceInput->text();
    int currentPrice = getPriceVal.toInt();
    if(isDivide || isMultiply || isSubtract || isAdd)
    {
        if(isDivide)
        {
            priceVal += currentPrice;
        }
        else if(isMultiply)
        {
            priceVal *= currentPrice;
        }
        else if(isSubtract)
        {
            priceVal -= currentPrice;
        }
        else if(isAdd)
        {
            priceVal += currentPrice;
        }
    }
    ui->priceInput->setText(QString::number(priceVal));
}

// Delete number one-by-one
void ListAddingDialog::on_del_button_clicked()
{
    ui->priceInput->backspace();
}

// Select all payer in the list
void ListAddingDialog::on_selectAll_button_clicked()
{
    for(int i{0}; i < nameButtonList.size(); i++)
    {
        nameButtonList.at(i)->setChecked(true);
    }
}

// Update list, total, price per person and add calculated amount to selected payer
void ListAddingDialog::on_done_button_clicked()
{
    priceVal = getPriceVal();
    int numPerson{0};
    int perPerson{0};

    // Check whether anyone is selected and price is entered
    for(int i{0}; i < nameButtonList.size(); i++)
    {
        if(nameButtonList.at(i)->isChecked())
        {
             numPerson++;
        }
    }

    if(numPerson > 0 && priceVal > 0)
    {
        perPerson = priceVal/numPerson;

        // Calculate amount per person within number of selected ones
        for(int i{0}; i < nameButtonList.size(); i++)
        {
            if((nameButtonList.at(i)->isChecked()) && (numPerson != 0))
            {

                payerList.at(i)->amountPerPerson(perPerson);
            }
        }

        // Set new price, price per person, total and close the dialog
        this->itemPtr->setNewPrice(priceVal);
        this->itemPtr->setNewPerPerson(perPerson);
        setNewTotal(priceVal);
        this->close();
    }
}
