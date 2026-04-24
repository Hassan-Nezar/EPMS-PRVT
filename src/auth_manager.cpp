#include "../include/auth_manager.h"


auth_manager::loginparser_result auth_manager::login_parser(string name, string id, string password) { //login parser method
    string first_name_construct = ""; 
    string last_name_construct = ""; 
    vector<char> first_name; 
    vector<char> last_name; 
    
    string id_1_construct = ""; 
    string id_2_construct = ""; 
    vector<char> id_1; 
    vector<char> id_2; 

    int name_length = name.length(); 
    bool whitespace_status = false; 

    for(int character = 0; character < name_length; character++) { 
        if (name.at(character) != ' ' && !whitespace_status) {
            first_name.push_back(name.at(character));
        } else if (name.at(character) != ' ' && whitespace_status) {
            last_name.push_back(name.at(character));
        } else {
            whitespace_status = true;
        } 
    }

    for (int character = 0; character < first_name.size(); character++) {
        first_name_construct += first_name.at(character);
    }
    for (int character = 0; character < last_name.size(); character++) {
        last_name_construct += last_name.at(character);
    }

    int id_length = id.length();
    for (int digit = 0; digit < id_length; digit++) {
        if (digit < 4) {
            id_1.push_back(id.at(digit));
        } else {
            id_2.push_back(id.at(digit));
        }
    }

    for(int digit = 0; digit < 4; digit++) {
        id_1_construct += id_1.at(digit);
        id_2_construct += id_2.at(digit);
    }
     
    loginparser_result r;
    r.firstnamer = first_name_construct;
    r.lastnamer = last_name_construct;
    r.id1r = id_1_construct;
    r.id2r = id_2_construct;
    r.passr = password;

    return r;
}

auth_manager::login_result auth_manager::login(string name, string id, string password) { //login method
    loginparser_result parsed = login_parser(name, id, password);
    login_result result;
    result.success = false;

    string fn, ln, ident, pass, pos;
    string ex1, ex2, ex3; 
    bool info_check = false;

    if (parsed.id1r == "0000") {
        pos = "Admin";
        ifstream file("program_files/admin.txt");
        
        while (file >> fn >> ln >> ident >> pass >> ex1) {
            if (parsed.firstnamer == fn && parsed.lastnamer == ln && ((parsed.id1r + parsed.id2r) == ident) && parsed.passr == pass) {
               info_check = true;
               result.extra1 = ex1; //for bio
               break;
            }
        }
    } else if (parsed.id1r == "9999") {
        pos = "Professor"; 
        ifstream file("program_files/professor.txt");
        
        while (file >> fn >> ln >> ident >> pass >> ex1 >> ex2) {
            if (parsed.firstnamer == fn && parsed.lastnamer == ln && ((parsed.id1r + parsed.id2r) == ident) && parsed.passr == pass) {
               info_check = true;
               result.extra1 = ex1; // for department
               result.extra2 = ex2; // for salary
               break;
            }
        }
    } else {
        pos = "Student";
        ifstream file("program_files/student.txt");
        
        while (file >> fn >> ln >> ident >> pass >> ex1 >> ex2 >> ex3) {
            if (parsed.firstnamer == fn && parsed.lastnamer == ln && ((parsed.id1r + parsed.id2r) == ident) && parsed.passr == pass) {
               info_check = true;
               result.extra1 = ex1; //for gpa
               result.extra2 = ex2; //for year
               result.extra3 = ex3; //for major
               break;
            }
        }
    }
    
    if (info_check) {
        result.success = true;
        result.first_name = fn;
        result.last_name = ln;
        result.id = ident;
        result.role = pos;
        cout << "Login is successful, welcome " << result.first_name << "!" << endl;
    } else {
        cout << "Login was unsuccessful." << endl;
    }

    return result;
}

void auth_manager::register_user() {
    string fname, lname, year, pass, major;
    int majorChoice;
    
    //inpuits
    cout << "Enter your First Name: "; 
    cin >> fname;
    cout << "Enter your Last Name: "; 
    cin >> lname;
    cout << "Enter your year (e.g., 1 for Freshman): "; 
    cin >> year;
    cout << "Enter your account password: "; 
    cin >> pass;

    cout << "\nChoose your Major:\n";
    cout << "1. Cyber_Security\n2. AI_Robotics\n3. Game_Design\n4. Astrophysics\n5. Bio_Informatics\n";
    cout << "6. Quantum_Computing\n7. Civil_Engineering\n8. Marine_Biology\n9. Software_Systems\n10. Data_Science\n";
    cout << "Choice: "; 
    cin >> majorChoice;

    // use index for input in array
    string majors[] = {"Unknown", "Cyber_Security", "AI_Robotics", "Game_Design", "Astrophysics", "Bio_Informatics", "Quantum_Computing", "Civil_Engineering", "Marine_Biology", "Software_Systems", "Data_Science"};
    
    if (majorChoice >= 1 && majorChoice <= 10) {
        major = majors[majorChoice];
    } else {
        major = "Undeclared"; 
    }

    //apend in register_queue.txt file
    ofstream file("program_files/register_queue.txt", ios::app);
    file << fname << " " << lname << " " << year << " " << pass << " " << major << endl;
    file.close();

    cout << "\n[Success] Application submitted! Please wait for Admin approval." << endl;
}