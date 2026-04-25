#pragma once
#include <QMainWindow>
#include <QDialog>
#include <QStackedWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QString>
#include <QVector>

class Courses;
class Authentication_manager;
class Courses_manager;
class View_search;

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);
    QString getUsername() const;
    QString getPassword() const;
    QString getRole()     const;
signals:
    void registerRequested();
private slots:
    void onLogin();
private:
    QLineEdit* m_user;
    QLineEdit* m_pass;
    QComboBox* m_role;
};

class RegisterDialog : public QDialog {
    Q_OBJECT
public:
    explicit RegisterDialog(QWidget* parent = nullptr);
private slots:
    void onSubmit();
private:
    QLineEdit* m_first, *m_last, *m_email, *m_pass;
    QComboBox* m_role;
};

class UI : public QMainWindow {
    Q_OBJECT
public:
    explicit UI(QWidget* parent = nullptr);
    ~UI() override;

    void showMainMenu();
    void showLoginPage();
    void showStudentMenu();
    void showProfessorMenu();
    void showAdminMenu();
    void showRegisterPage();
    void showCourseListPage(const QVector<Courses*>& courses);
    void printMessage(const QString& msg);

    void setAuthManager   (Authentication_manager* m);
    void setCoursesManager(Courses_manager*        m);
    void setSearchManager (View_search*            m);

private slots:
    void onLoginAccepted(const QString& user, const QString& pass, const QString& role);
    void onLogoutClicked();
    void onNavItemClicked(QListWidgetItem* item);
    void onSearchClicked();

private:
    void     buildTopBar();
    void     buildSidebar(const QString& role);
    void     buildAllPages(const QString& role);
    void     showPage(const QString& key);
    QWidget* makePage(const QString& title);
    QWidget* makeMetricCard(const QString& label, const QString& value, const QString& color = "#185FA5");
    QWidget* buildDashboardPage();
    QWidget* buildCoursesPage();
    QWidget* buildSearchPage();
    QWidget* buildProfilePage();

    QWidget*        m_topBar  = nullptr;
    QListWidget*    m_sidebar = nullptr;
    QStackedWidget* m_stack   = nullptr;
    QString         m_role, m_username;

    Authentication_manager* m_authMgr = nullptr;
    Courses_manager*        m_crseMgr = nullptr;
    View_search*            m_srchMgr = nullptr;
};
