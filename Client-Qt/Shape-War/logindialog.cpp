#include "logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setUpGUI();
    setWindowTitle(tr("User Login"));
    setModal(true);
}

void LoginDialog::setUpGUI() {
    // set up the layout
    QGridLayout *formGridLayout = new QGridLayout(this);

    editServerUrl = new QLineEdit(this);
    editPort = new QLineEdit(this);
    editUsername = new QLineEdit(this);
    // initialize the password field so that it does not echo characters
    editPassword = new QLineEdit(this);
    editPassword->setEchoMode(QLineEdit::Password);

    // initialize the labels
    labelServerUrl = new QLabel(this);
    labelServerUrl->setText(tr("ServerUrl"));
    labelServerUrl->setBuddy(editServerUrl);

    labelPort = new QLabel(this);
    labelPort->setText(tr("Port"));
    labelPort->setBuddy(editPort);

    labelUsername = new QLabel(this);
    labelUsername->setText(tr("Username"));
    labelUsername->setBuddy(editUsername);

    labelPassword = new QLabel(this);
    labelPassword->setText(tr("Password"));
    labelPassword->setBuddy(editPassword);


    // initialize buttons
    buttons = new QDialogButtonBox(this);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("Login"));
    buttons->button(QDialogButtonBox::Cancel)->setText(tr("Abort"));

    // connects slots
    connect(buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this,
            SLOT(close()));

    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this,
            SLOT(slotAcceptLogin()));

    anonymousCheck = new QCheckBox("Play anonymous", this);
    connect(anonymousCheck, SIGNAL(clicked()), this,
            SLOT(anonymousCheckOnclicked()));

    // place components into the dialog
    int rowCounting = 0;
    formGridLayout->addWidget(labelServerUrl, rowCounting, 0);
    formGridLayout->addWidget(editServerUrl, rowCounting, 1);
    rowCounting++;
    formGridLayout->addWidget(labelPort, rowCounting, 0);
    formGridLayout->addWidget(editPort, rowCounting, 1);
    rowCounting++;
    formGridLayout->addWidget(labelUsername, rowCounting, 0);
    formGridLayout->addWidget(editUsername, rowCounting, 1);
    rowCounting++;
    formGridLayout->addWidget(labelPassword, rowCounting, 0);
    formGridLayout->addWidget(editPassword, rowCounting, 1);
    rowCounting++;
    formGridLayout->addWidget(anonymousCheck, rowCounting, 0);
    rowCounting++;
    formGridLayout->addWidget(buttons, rowCounting, 0, 1, 2);

    setLayout(formGridLayout);
}

void LoginDialog::setPassword(QString &password) {
    editPassword->setText(password);
}

void LoginDialog::slotAcceptLogin() {
    QString serverUrl = editServerUrl->text();
    QString port = editPort->text();
    QString username = editUsername->text();
    QString password = editPassword->text();
    bool isAnonymous = anonymousCheck->isChecked();
    emit acceptLogin(serverUrl,
                     port,
                     username, // current username
                     password,  // current password
                     isAnonymous
                     );

    // close this dialog
    close();
}
void LoginDialog::anonymousCheckOnclicked() {
    if( anonymousCheck->isChecked() ) {
        editPassword->setEnabled(false);
    } else {
        editPassword->setEnabled(true);
    }
}
