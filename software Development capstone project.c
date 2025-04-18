#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// File definitions
#define ADMIN_FILE "admins.dat"
#define REPORT_FILE "reports.txt"
#define EMPLOYEE_REPORTS_FILE "employee_reports.txt"
#define ATTENDANCE_FILE "attendance.txt"
#define PAYROLL_FILE "payroll_data.txt"
#define LEAVE_FILE "leave_records.txt"

// Constants
#define WORKING_HOURS_PER_DAY 8
#define MAX_LEAVE_DAYS 30

// Structure definitions
typedef struct {
    int id;
    char name[50];
    char contact[15];
    char username[50];
    char password[50];
} Admin;

typedef struct {
    char reportType[20];
    char date[15];
    char details[200];
    char filter[50];
} Report;

typedef struct {
    int reportID;
    int empID;
    char name[50];
    char date[15];
    char reportType[20];
    char details[500];
    char status[20];
} EmployeeReport;

typedef struct {
    char empID[10];
    char name[50];
    char date[15];
    char status[10];
} Attendance;

typedef struct {
    int id;
    char name[50];
    char department[30];
    float hourly_rate;
    float overtime_rate;
    float bonus;
    float deduction;
    int days_worked;
} Employee;

typedef struct {
    int empID;
    char name[50];
    char leaveType[20];
    char startDate[15];
    char endDate[15];
    char status[15];
    int leaveDays;
} LeaveRecord;

// Global variables
int role = 0; // 1=Manager, 2=Admin, 3=Employee
char currentUser[50];
int currentEmpID = 0;

// Function prototypes
void admin_management_menu();
void report_management_menu();
void loginSystem();
void manager_menu();
void admin_menu();
void employee_menu();

// Admin Management Functions
void addAdmin();
void viewAdmins();
void deleteAdmin();

// Report Management Functions
void generateAttendanceReport();
void generateSalaryReport();
void generateLeaveReport();
void viewReports();
void submitEmployeeReport();
void viewEmployeeReports();

// Helper Functions
void clearInputBuffer();

int main() {
    while(1) {
        loginSystem();

        if (role == 1) { // Manager
            manager_menu();
        }
        else if (role == 2) { // Admin
            admin_menu();
        }
        else if (role == 3) { // Employee
            employee_menu();
        }
    }
    return 0;
}

void manager_menu() {
    int choice;
    while (1) {
        printf("\n=== MANAGER MENU ===\n");
        printf("1. Admin Management\n");
        printf("2. Report Management\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: admin_management_menu(); break;
            case 2: report_management_menu(); break;
            case 3: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void admin_menu() {
    int choice;
    while (1) {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Report Management\n");
        printf("2. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: report_management_menu(); break;
            case 2: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void employee_menu() {
    int choice;
    while (1) {
        printf("\n=== EMPLOYEE MENU ===\n");
        printf("1. Report Management\n");
        printf("2. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: report_management_menu(); break;
            case 2: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void loginSystem() {
    char username[50], password[50];
    printf("\n=== LOGIN SYSTEM ===\n");
    printf("Enter Username: ");
    scanf("%49s", username);
    printf("Enter Password: ");
    scanf("%49s", password);
    clearInputBuffer();

    strcpy(currentUser, username);

    // Simple authentication
    if (strcmp(username, "manager") == 0 && strcmp(password, "mgr123") == 0) {
        role = 1;
        currentEmpID = 1001;
        printf("\nManager login successful!\n");
    }
    else if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        role = 2;
        currentEmpID = 1002;
        printf("\nAdmin login successful!\n");
    }
    else if (strcmp(username, "employee") == 0 && strcmp(password, "emp123") == 0) {
        role = 3;
        currentEmpID = 1003;
        printf("\nEmployee login successful!\n");
    }
    else {
        printf("\nInvalid credentials!\n");
        exit(1);
    }
}

// ================= ADMIN MANAGEMENT FUNCTIONS =================

void admin_management_menu() {
    int choice;
    while (1) {
        printf("\n=== ADMIN MANAGEMENT ===\n");
        printf("1. Add Admin\n");
        printf("2. View Admins\n");
        printf("3. Delete Admin\n");
        printf("4. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addAdmin(); break;
            case 2: viewAdmins(); break;
            case 3: deleteAdmin(); break;
            case 4: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void addAdmin() {
    if (role != 1) {
        printf("Access Denied! Only Managers can add admins.\n");
        return;
    }

    Admin newAdmin;
    memset(&newAdmin, 0, sizeof(Admin));

    printf("\n=== ADD NEW ADMIN ===\n");
    printf("Enter username: ");
    scanf("%49s", newAdmin.username);
    printf("Enter password: ");
    scanf("%49s", newAdmin.password);
    clearInputBuffer();

    FILE *file = fopen(ADMIN_FILE, "ab");
    if (!file) {
        printf("Error opening admin file!\n");
        return;
    }

    if (fwrite(&newAdmin, sizeof(Admin), 1, file) != 1) {
        printf("Error saving admin!\n");
    } else {
        printf("Admin added successfully!\n");
    }
    fclose(file);
}

void viewAdmins() {
    if (role != 1) {
        printf("Access Denied! Only Managers can view admins.\n");
        return;
    }

    FILE *file = fopen(ADMIN_FILE, "rb");
    if (!file) {
        printf("No admins found!\n");
        return;
    }

    printf("\n=== ADMINISTRATORS ===\n");
    printf("----------------------------------------\n");
    printf("Default Admin: username=admin, password=admin123\n");
    printf("----------------------------------------\n");

    Admin admin;
    while (fread(&admin, sizeof(Admin), 1, file) == 1) {
        printf("Username: %-15s Password: %s\n", admin.username, admin.password);
    }
    fclose(file);
}

void deleteAdmin() {
    if (role != 1) {
        printf("Access Denied! Only Managers can delete admins.\n");
        return;
    }

    FILE *file = fopen(ADMIN_FILE, "rb");
    if (!file) {
        printf("No admins found!\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    char username[50];
    int found = 0;
    Admin admin;

    printf("\nEnter Admin Username to delete: ");
    scanf("%49s", username);
    clearInputBuffer();

    while (fread(&admin, sizeof(Admin), 1, file)) {
        if (strcmp(admin.username, username) != 0) {
            fwrite(&admin, sizeof(Admin), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove(ADMIN_FILE);
    rename("temp.dat", ADMIN_FILE);

    if (found) {
        printf("Admin '%s' deleted successfully!\n", username);
    } else {
        printf("Admin with username '%s' not found!\n", username);
    }
}

// ================= REPORT MANAGEMENT FUNCTIONS =================

void report_management_menu() {
    int choice;
    while (1) {
        printf("\n=== REPORT MANAGEMENT ===\n");

        if (role == 1 || role == 2) { // Manager or Admin
            printf("1. Generate Attendance Report\n");
            printf("2. Generate Salary Report\n");
            printf("3. Generate Leave Report\n");
            printf("4. View All Reports\n");
            printf("5. Submit Employee Report\n");
            printf("6. View My Reports\n");
            printf("7. Return to Main Menu\n");
        } else { // Employee
            printf("1. Submit Employee Report\n");
            printf("2. View My Reports\n");
            printf("3. Return to Main Menu\n");
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (role == 1 || role == 2) generateAttendanceReport();
                else submitEmployeeReport();
                break;
            case 2:
                if (role == 1 || role == 2) generateSalaryReport();
                else viewEmployeeReports();
                break;
            case 3:
                if (role == 1 || role == 2) generateLeaveReport();
                else return;
                break;
            case 4:
                if (role == 1 || role == 2) viewReports();
                else printf("Invalid choice!\n");
                break;
            case 5:
                if (role == 1 || role == 2) submitEmployeeReport();
                else printf("Invalid choice!\n");
                break;
            case 6:
                if (role == 1 || role == 2) viewEmployeeReports();
                else printf("Invalid choice!\n");
                break;
            case 7:
                if (role == 1 || role == 2) return;
                else printf("Invalid choice!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}

void generateAttendanceReport() {
    FILE *attFile = fopen(ATTENDANCE_FILE, "r");
    FILE *reportFile = fopen(REPORT_FILE, "a");
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (!attFile || !reportFile) {
        printf("Error opening files!\n");
        return;
    }

    char filter[50];
    printf("\nEnter filter (All/Department/EmployeeID): ");
    scanf("%49s", filter);
    clearInputBuffer();

    Attendance att;
    int present = 0, absent = 0;
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", t);

    fprintf(reportFile, "Attendance Report,%s,%s\n", dateStr, filter);

    while (fscanf(attFile, "%[^,],%[^,],%[^,],%s\n",
                 att.empID, att.name, att.date, att.status) != EOF) {
        if (strcmp(filter, "All") == 0 ||
            strstr(att.name, filter) != NULL ||
            strcmp(att.empID, filter) == 0) {
            if (strcmp(att.status, "Present") == 0) present++;
            else absent++;
        }
    }

    fprintf(reportFile, "Present: %d, Absent: %d\n", present, absent);
    fclose(attFile);
    fclose(reportFile);
    printf("\nAttendance report generated successfully!\n");
}

void generateSalaryReport() {
    FILE *salFile = fopen(PAYROLL_FILE, "r");
    FILE *reportFile = fopen(REPORT_FILE, "a");
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (!salFile || !reportFile) {
        printf("Error opening files!\n");
        return;
    }

    Employee emp;
    float totalSalary = 0;
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", t);

    fprintf(reportFile, "Salary Report,%s,All\n", dateStr);

    while (fscanf(salFile, "%d,%49[^,],%29[^,],%f,%f,%f,%f,%d",
                 &emp.id, emp.name, emp.department, &emp.hourly_rate,
                 &emp.overtime_rate, &emp.bonus, &emp.deduction, &emp.days_worked) != EOF) {
        float salary = emp.hourly_rate * WORKING_HOURS_PER_DAY * emp.days_worked;
        totalSalary += salary;
        fprintf(reportFile, "%d,%s,%.2f\n", emp.id, emp.name, salary);
    }

    fprintf(reportFile, "Total Salary: %.2f\n", totalSalary);
    fclose(salFile);
    fclose(reportFile);
    printf("\nSalary report generated successfully!\n");
}

void generateLeaveReport() {
    FILE *leaveFile = fopen(LEAVE_FILE, "r");
    FILE *reportFile = fopen(REPORT_FILE, "a");
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (!leaveFile || !reportFile) {
        printf("Error opening files!\n");
        return;
    }

    LeaveRecord lv;
    int approved = 0, rejected = 0, pending = 0;
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", t);

    fprintf(reportFile, "Leave Report,%s,All\n", dateStr);

    while (fscanf(leaveFile, "%d,%49[^,],%19[^,],%14[^,],%14[^,],%14[^,],%d\n",
                &lv.empID, lv.name, lv.leaveType,
                lv.startDate, lv.endDate, lv.status, &lv.leaveDays) != EOF) {
        if (strcmp(lv.status, "Approved") == 0) approved++;
        else if (strcmp(lv.status, "Rejected") == 0) rejected++;
        else pending++;
    }

    fprintf(reportFile, "Approved: %d, Rejected: %d, Pending: %d\n", approved, rejected, pending);
    fclose(leaveFile);
    fclose(reportFile);
    printf("\nLeave report generated successfully!\n");
}

void viewReports() {
    FILE *file = fopen(REPORT_FILE, "r");
    if (!file) {
        printf("No reports found!\n");
        return;
    }

    printf("\n=== SYSTEM REPORTS ===\n");
    printf("----------------------------------------\n");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("----------------------------------------\n");
    fclose(file);
}

void submitEmployeeReport() {
    FILE *file = fopen(EMPLOYEE_REPORTS_FILE, "a");
    if (!file) {
        printf("Error opening employee reports file!\n");
        return;
    }

    EmployeeReport report;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(report.date, sizeof(report.date), "%d/%m/%Y", t);

    report.reportID = rand() % 10000 + 1;
    report.empID = currentEmpID;

    printf("\n=== SUBMIT EMPLOYEE REPORT ===\n");
    printf("Enter your name: ");
    scanf(" %49[^\n]", report.name);
    clearInputBuffer();

    printf("Report type (Complaint/Suggestion/Feedback): ");
    scanf(" %19[^\n]", report.reportType);
    clearInputBuffer();

    printf("Enter details (max 500 chars): ");
    scanf(" %499[^\n]", report.details);
    clearInputBuffer();

    strcpy(report.status, "Pending");

    fprintf(file, "%d,%d,%s,%s,%s,%s,%s\n",
            report.reportID, report.empID, report.name,
            report.date, report.reportType, report.details, report.status);

    fclose(file);
    printf("\nReport submitted successfully! Your Report ID: %d\n", report.reportID);
}

void viewEmployeeReports() {
    FILE *file = fopen(EMPLOYEE_REPORTS_FILE, "r");
    if (!file) {
        printf("No reports found for you!\n");
        return;
    }

    printf("\n=== YOUR REPORTS ===\n");
    printf("------------------------------------------------------------\n");
    printf("ID\tDate\t\tType\t\tStatus\n");
    printf("------------------------------------------------------------\n");

    EmployeeReport report;
    while (fscanf(file, "%d,%d,%49[^,],%14[^,],%19[^,],%499[^,],%19[^\n]",
                 &report.reportID, &report.empID, report.name,
                 report.date, report.reportType, report.details, report.status) != EOF) {
        if (report.empID == currentEmpID) {
            printf("%d\t%s\t%-12s\t%s\n",
                  report.reportID, report.date, report.reportType, report.status);
        }
    }
    printf("------------------------------------------------------------\n");
    fclose(file);
}

// ================= HELPER FUNCTIONS =================

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
