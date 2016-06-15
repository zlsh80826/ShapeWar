#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QComboBox>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QCheckBox>
#include <QSpacerItem>

/*!
 '''Makes class LoginDialog a child to its parent, QDialog
*/
class LoginDialog : public QDialog {
    Q_OBJECT
private:
    QLabel *labelServerIP;
    QLabel *labelPort;
    QLabel *labelUsername;
    QLabel *labelPassword;

    QLineEdit *editServerIP;
    QLineEdit *editPort;
    QLineEdit *editUsername;
    QLineEdit *editPassword;

    QCheckBox *anonymousCheck;
    /*!
     * The standard dialog button box.
     */
    QDialogButtonBox *buttons;

    /*!
     * A method to set up all dialog components and
     * initialize them.
     */
    void setUpGUI();

public:
    explicit LoginDialog(QWidget *parent = 0);

    /*!
    * Sets the proposed username, that can come for instance
    * from a shared setting.
    '''username the string that represents the current username
   ''' to display
    */
    void setUsername(QString &username);

    /*!
    * Sets the current password to propose to the user for the login.
    * password the password to fill into the dialog form
    */
    void setPassword(QString &password);

    /*!
    * Sets a list of allowed usernames from which the user
    * can pick one if he does not want to directly edit it.
    '''usernames a list of usernames
   */
    void setUsernamesList(const QStringList &usernames);

signals:
    /*!
     * A signal emitted when the login is performed.
     * username the username entered in the dialog
     * password the password entered in the dialog
     * index the number of the username selected in the combobox
     */
    void acceptLogin(QString &ServerIP, QString & port, QString &username, QString &password, bool isAnonymous);
public slots:
    void slotAcceptLogin();
    void anonymousCheckOnclicked();
};

#endif // LOGINDIALOG_H
