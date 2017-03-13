#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QModelIndex>
#include <QListWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    /**
     * @brief on click log in if the login and password are correct
     * on click hide the login password fields and show the "choose a base" comboBox if the login and password fields are correct
     */
    void on_pushButtonValider_clicked();

    /**
     * @brief on activation select the database selected
     * on activation seclect the base then hide the field and show the user interface that let him type requests save and load them
     * @param arg1
     */
    void on_comboBoxDbList_activated(const QString &arg1);

    /**
     * @brief on click on an item (a table name) of the list show the fields that are in this table
     * @param item
     */
    void on_listWidgetTables_itemClicked(QListWidgetItem *item);

    /**
     * @brief on click show the error message if there is an error in the request, else show a message who says that everything is good
     */
    void on_pushButtonInfo_clicked();

    /**
     * @brief on click on an item of the list write it into the request field and executes it
     * @param item
     */
    void on_listWidgetHistory_itemDoubleClicked(QListWidgetItem *item);

    /**
     * @brief on edit execute the request typed
     * if there's no ";" add it at the end of the text, if the request works show the result into the table under the textEdit field and change the pushButtonInfo into a green cross and enable the pushButtonSaveHistory
     * else change the pushButtonInfo into a red cross
     */
    void on_textEditReqSQL_textChanged();

    /**
     * @brief on push add the request on the textEdit into the list
     */
    void on_pushButtonSaveHistory_clicked();

    /**
     * @brief on push save the history on an external file in a .sql format
     * on push open a window that let you choose a file that you either erase or create to save your history list in it in a .sql format
     */
    void on_pushButtonSave_clicked();

    /**
     * @brief on push load a file .sql and put every single requests in it on the list
     */
    void on_pushButtonLoad_clicked();

    void on_pushButtonValidateServer_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase maBase;
};

#endif // MAINWINDOW_H
