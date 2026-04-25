#include "ui2.h"
#include <QApplication>
#include <QHeaderView>
#include <QFrame>

static const char* S_PRIMARY = "background:#185FA5;color:white;border:none;border-radius:4px;padding:6px 14px;";
static const char* S_SMALL   = "border:0.5px solid #d0d0d0;border-radius:4px;padding:3px 10px;";

LoginDialog::LoginDialog(QWidget* p) : QDialog(p) {
    setWindowTitle("Login"); setFixedSize(320, 270);
    auto* r=new QVBoxLayout(this); r->setContentsMargins(24,24,24,24); r->setSpacing(12);
    r->addWidget(new QLabel("<b style='font-size:15px;'>Zewail <span style='color:#185FA5;'>UniSys</span></b>"));
    auto* f=new QFormLayout(); f->setSpacing(8);
    m_user=new QLineEdit(); m_pass=new QLineEdit(); m_pass->setEchoMode(QLineEdit::Password);
    m_role=new QComboBox(); m_role->addItems({"Student","Professor","Admin"});
    f->addRow("Username:",m_user); f->addRow("Password:",m_pass); f->addRow("Role:",m_role);
    r->addLayout(f);
    auto* ok=new QPushButton("Login"); ok->setStyleSheet(S_PRIMARY); ok->setDefault(true);
    auto* reg=new QPushButton("Register"); reg->setFlat(true); reg->setStyleSheet("color:#185FA5;border:none;");
    r->addWidget(ok); r->addWidget(reg);
    connect(ok,  &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(reg, &QPushButton::clicked, this, &LoginDialog::registerRequested);
}
void LoginDialog::onLogin() {
    if (m_user->text().isEmpty()) { QMessageBox::warning(this,"Login","Fill all fields."); return; }
    accept();
}
QString LoginDialog::getUsername() const { return m_user->text().trimmed(); }
QString LoginDialog::getPassword() const { return m_pass->text(); }
QString LoginDialog::getRole()     const { return m_role->currentText().toLower(); }

RegisterDialog::RegisterDialog(QWidget* p) : QDialog(p) {
    setWindowTitle("Register"); setFixedSize(340, 300);
    auto* r=new QVBoxLayout(this); r->setContentsMargins(24,24,24,24); r->setSpacing(10);
    r->addWidget(new QLabel("<b style='font-size:15px;'>Create Account</b>"));
    auto* f=new QFormLayout(); f->setSpacing(8);
    m_first=new QLineEdit(); m_last=new QLineEdit(); m_email=new QLineEdit();
    m_pass=new QLineEdit(); m_pass->setEchoMode(QLineEdit::Password);
    m_role=new QComboBox(); m_role->addItems({"Student","Professor"});
    f->addRow("First:",m_first); f->addRow("Last:",m_last);
    f->addRow("Email:",m_email); f->addRow("Password:",m_pass); f->addRow("Role:",m_role);
    r->addLayout(f);
    auto* b=new QHBoxLayout();
    auto* ok=new QPushButton("Register"); ok->setStyleSheet(S_PRIMARY);
    auto* back=new QPushButton("Back");
    b->addWidget(ok); b->addWidget(back); r->addLayout(b);
    connect(ok,   &QPushButton::clicked, this, &RegisterDialog::onSubmit);
    connect(back, &QPushButton::clicked, this, &QDialog::reject);
}
void RegisterDialog::onSubmit() {
    if (m_first->text().isEmpty()||m_email->text().isEmpty()) { QMessageBox::warning(this,"Register","Fill all fields."); return; }
    QMessageBox::information(this,"Register","Submitted. Awaiting admin approval.");
    accept();
}

UI::UI(QWidget* p) : QMainWindow(p) {
    setWindowTitle("Zewail UniSys"); setMinimumSize(900,620); showLoginPage();
}
UI::~UI() {}

void UI::setAuthManager   (Authentication_manager* m) { m_authMgr = m; }
void UI::setCoursesManager(Courses_manager*        m) { m_crseMgr = m; }
void UI::setSearchManager (View_search*            m) { m_srchMgr = m; }
void UI::printMessage(const QString& msg) { QMessageBox::information(this,"UniSys",msg); }

void UI::showLoginPage() {
    auto* d=new LoginDialog(this);
    connect(d, &LoginDialog::registerRequested, this, &UI::showRegisterPage);
    if (d->exec()==QDialog::Accepted) onLoginAccepted(d->getUsername(),d->getPassword(),d->getRole());
    else QApplication::quit();
    d->deleteLater();
}
void UI::showRegisterPage() { auto* d=new RegisterDialog(this); d->exec(); d->deleteLater(); }
void UI::showStudentMenu()   { m_role="student";   showMainMenu(); }
void UI::showProfessorMenu() { m_role="professor"; showMainMenu(); }
void UI::showAdminMenu()     { m_role="admin";     showMainMenu(); }
void UI::showCourseListPage(const QVector<Courses*>&) { showPage("courses"); }

void UI::onLoginAccepted(const QString& user, const QString&, const QString& role) {
    m_username=user; m_role=role; showMainMenu();
}

void UI::showMainMenu() {
    auto* c=new QWidget(this);
    auto* root=new QVBoxLayout(c); root->setSpacing(0); root->setContentsMargins(0,0,0,0);
    buildTopBar(); root->addWidget(m_topBar);
    auto* body=new QHBoxLayout(); body->setSpacing(0);
    m_sidebar=new QListWidget(); m_sidebar->setFixedWidth(185);
    m_sidebar->setStyleSheet(
        "QListWidget{border:none;background:#fafafa;border-right:1px solid #e8e8e8;}"
        "QListWidget::item{padding:8px 14px;font-size:13px;}"
        "QListWidget::item:selected{background:#E6F1FB;color:#185FA5;border-left:2px solid #185FA5;font-weight:bold;}"
        "QListWidget::item:hover:!selected{background:#f0f0f0;}");
    buildSidebar(m_role);
    m_stack=new QStackedWidget();
    buildAllPages(m_role);
    body->addWidget(m_sidebar); body->addWidget(m_stack,1);
    root->addLayout(body,1);
    setCentralWidget(c);
    connect(m_sidebar,&QListWidget::itemClicked,this,&UI::onNavItemClicked);
    if (m_sidebar->count()>0) { m_sidebar->setCurrentRow(0); showPage("dashboard"); }
}

void UI::buildTopBar() {
    m_topBar=new QWidget(); m_topBar->setFixedHeight(46);
    m_topBar->setStyleSheet("background:white;border-bottom:1px solid #e8e8e8;");
    auto* l=new QHBoxLayout(m_topBar); l->setContentsMargins(14,0,14,0);
    l->addWidget(new QLabel("<b>Zewail</b> <span style='color:#185FA5;font-weight:bold;'>UniSys</span>"));
    l->addStretch();
    l->addWidget(new QLabel(m_username+" ["+m_role+"]"));
    auto* out=new QPushButton("Logout"); out->setStyleSheet(S_SMALL);
    l->addWidget(out);
    connect(out,&QPushButton::clicked,this,&UI::onLogoutClicked);
}

void UI::buildSidebar(const QString& role) {
    m_sidebar->clear();
    auto item=[&](const QString& lbl,const QString& pg){
        auto* i=new QListWidgetItem(lbl,m_sidebar); i->setData(Qt::UserRole,pg);};
    auto sep=[&](const QString& t){
        auto* i=new QListWidgetItem("  "+t.toUpper(),m_sidebar);
        i->setFlags(Qt::NoItemFlags); i->setForeground(QColor("#aaa"));
        QFont f=i->font(); f.setPointSize(9); i->setFont(f);};
    item("  Dashboard","dashboard");
    sep("Academic");
    item("  Courses","courses");
    sep("Tools");
    item("  Search","search");
    item("  Profile","profile");
    Q_UNUSED(role);
}

void UI::buildAllPages(const QString& role) {
    Q_UNUSED(role);
    auto add=[&](const QString& k,QWidget* w){ w->setObjectName(k); m_stack->addWidget(w); };
    add("dashboard", buildDashboardPage());
    add("courses",   buildCoursesPage());
    add("search",    buildSearchPage());
    add("profile",   buildProfilePage());
}

void UI::showPage(const QString& key) {
    for (int i=0;i<m_stack->count();++i)
        if (m_stack->widget(i)->objectName()==key) { m_stack->setCurrentIndex(i); return; }
}
void UI::onNavItemClicked(QListWidgetItem* item) {
    QString k=item->data(Qt::UserRole).toString(); if (!k.isEmpty()) showPage(k);
}
void UI::onLogoutClicked() {
    setCentralWidget(nullptr); m_topBar=nullptr; m_sidebar=nullptr; m_stack=nullptr;
    showLoginPage();
}

QWidget* UI::makePage(const QString& title) {
    auto* p=new QWidget();
    auto* l=new QVBoxLayout(p); l->setContentsMargins(20,20,20,20); l->setSpacing(12);
    l->addWidget(new QLabel("<b style='font-size:16px;'>"+title+"</b>"));
    return p;
}
QWidget* UI::makeMetricCard(const QString& label,const QString& value,const QString& color) {
    auto* c=new QFrame(); c->setStyleSheet("background:#f0f4f8;border-radius:8px;"); c->setMinimumHeight(72);
    auto* l=new QVBoxLayout(c); l->setContentsMargins(12,10,12,10); l->setSpacing(4);
    auto* lb=new QLabel(label); lb->setStyleSheet("color:gray;font-size:11px;");
    auto* vl=new QLabel(value); vl->setStyleSheet("font-size:22px;font-weight:bold;color:"+color+";");
    l->addWidget(lb); l->addWidget(vl); return c;
}

QWidget* UI::buildDashboardPage() {
    auto* p=makePage("Dashboard");
    auto* vl=static_cast<QVBoxLayout*>(p->layout());
    auto* g=new QGridLayout(); g->setSpacing(10);
    if (m_role=="student") {
        g->addWidget(makeMetricCard("GPA","3.75","#185FA5"),0,0);
        g->addWidget(makeMetricCard("Courses","4","#3B6D11"),0,1);
        g->addWidget(makeMetricCard("Social points","87","#854F0B"),0,2);
        g->addWidget(makeMetricCard("Open cases","2","#A32D2D"),0,3);
    } else if (m_role=="professor") {
        g->addWidget(makeMetricCard("My courses","2","#185FA5"),0,0);
        g->addWidget(makeMetricCard("Students","62","#3B6D11"),0,1);
        g->addWidget(makeMetricCard("Avg grade","B+","#854F0B"),0,2);
        g->addWidget(makeMetricCard("Pending","5","#A32D2D"),0,3);
    } else {
        g->addWidget(makeMetricCard("Students","120","#185FA5"),0,0);
        g->addWidget(makeMetricCard("Professors","18","#3B6D11"),0,1);
        g->addWidget(makeMetricCard("Courses","32","#854F0B"),0,2);
        g->addWidget(makeMetricCard("Pending","3","#A32D2D"),0,3);
    }
    vl->addLayout(g);
    auto* act=new QGroupBox("Recent Activity"); auto* al=new QVBoxLayout(act);
    for (auto& a : QStringList{"Grade posted – CS 151  (2h ago)","Enrollment confirmed – MATH 201  (1d ago)","Attendance marked – CS 301  (2d ago)"})
        al->addWidget(new QLabel(a));
    vl->addWidget(act); vl->addStretch(); return p;
}

QWidget* UI::buildCoursesPage() {
    auto* p=makePage("Courses");
    auto* vl=static_cast<QVBoxLayout*>(p->layout());
    auto* search=new QLineEdit(); search->setPlaceholderText("Search...");
    vl->addWidget(search);
    auto* t=new QTableWidget(0,5);
    t->setHorizontalHeaderLabels({"Code","Name","Credits","Professor","Seats"});
    t->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->verticalHeader()->setVisible(false);
    struct R{QString code,name,prof;int cr,seats;};
    for (auto& d : QVector<R>{{"CS301","Data Structures","Dr. Elshamy",3,5},
        {"CS405","Algorithms","Dr. Farouk",3,2},
        {"CS151","OOP","Dr. Elshamy",3,10},
        {"MATH201","Linear Algebra","Dr. Farouk",3,8},
        {"CS210","Operating Systems","Dr. Elshamy",3,0}}) {
        int r=t->rowCount(); t->insertRow(r);
        t->setItem(r,0,new QTableWidgetItem(d.code));
        t->setItem(r,1,new QTableWidgetItem(d.name));
        t->setItem(r,2,new QTableWidgetItem(QString::number(d.cr)));
        t->setItem(r,3,new QTableWidgetItem(d.prof));
        t->setItem(r,4,new QTableWidgetItem(d.seats==0?"Full":QString::number(d.seats)));
    }
    vl->addWidget(t); return p;
}

QWidget* UI::buildSearchPage() {
    auto* p=makePage("Search");
    auto* vl=static_cast<QVBoxLayout*>(p->layout());
    auto* row=new QHBoxLayout();
    auto* input=new QLineEdit(); input->setObjectName("searchInput"); input->setPlaceholderText("Search...");
    auto* scope=new QComboBox(); scope->setObjectName("searchScope");
    scope->addItems({"Global","Students only","Courses only","Professors only"});
    auto* btn=new QPushButton("Search"); btn->setStyleSheet(S_PRIMARY);
    row->addWidget(input,1); row->addWidget(scope); row->addWidget(btn);
    vl->addLayout(row);
    auto* t=new QTableWidget(0,3); t->setObjectName("searchResults");
    t->setHorizontalHeaderLabels({"Type","Name","Detail"});
    t->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers); t->verticalHeader()->setVisible(false);
    vl->addWidget(t);
    connect(btn,&QPushButton::clicked,this,&UI::onSearchClicked);
    vl->addStretch(); return p;
}

QWidget* UI::buildProfilePage() {
    auto* p=makePage("Profile");
    auto* box=new QGroupBox(); auto* f=new QFormLayout(box);
    f->addRow("Username:",new QLabel(m_username));
    f->addRow("Role:",    new QLabel(m_role));
    if      (m_role=="student")   { f->addRow("GPA:",new QLabel("3.75")); f->addRow("Major:",new QLabel("AI")); }
    else if (m_role=="professor") { f->addRow("Dept:",new QLabel("Computer Science")); }
    else                          { f->addRow("Privilege:",new QLabel("superadmin")); }
    static_cast<QVBoxLayout*>(p->layout())->addWidget(box);
    static_cast<QVBoxLayout*>(p->layout())->addStretch(); return p;
}

void UI::onSearchClicked() {
    auto* pg=m_stack->currentWidget();
    auto* input=pg->findChild<QLineEdit*>("searchInput");
    auto* scope=pg->findChild<QComboBox*>("searchScope");
    auto* res  =pg->findChild<QTableWidget*>("searchResults");
    if (!input||!res) return;
    QString q=input->text().trimmed().toLower(); if (q.isEmpty()) return;
    res->setRowCount(0);
    struct R{QString type,name,detail;};
    for (auto& d : QVector<R>{{"Student","Hassan Nezar","ID:202505314 GPA:3.75"},
        {"Student","Islam Taraa","ID:202500225 GPA:3.40"},
        {"Course","CS301 Data Structures","3cr seats:5"},
        {"Professor","Dr. Elshamy","m.elshamy@zewailcity.edu.eg"}}) {
        QString sc=scope->currentText();
        if (sc=="Students only"  &&d.type!="Student")   continue;
        if (sc=="Courses only"   &&d.type!="Course")    continue;
        if (sc=="Professors only"&&d.type!="Professor") continue;
        if (!d.name.toLower().contains(q)&&!d.detail.toLower().contains(q)) continue;
        int r=res->rowCount(); res->insertRow(r);
        res->setItem(r,0,new QTableWidgetItem(d.type));
        res->setItem(r,1,new QTableWidgetItem(d.name));
        res->setItem(r,2,new QTableWidgetItem(d.detail));
    }
}
