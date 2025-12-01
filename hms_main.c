#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 64
#define PAT_FILE "patients.dat"
#define DOC_FILE "doctors.dat"
#define APPT_FILE "appointments.dat"
#define BILL_FILE "bills.dat"

/* CENTER TEXT FUNCTION */
void centerText(const char *text) {
    int width = 80;  
    int len = strlen(text);
    int spaces = (width - len) / 2;
    for (int i = 0; i < spaces; i++) printf(" ");
    printf("%s\n", text);
}

/* Data structures */

typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
    char gender[16];
    char contact[32];
} Patient;

typedef struct {
    int id;
    char name[MAX_NAME];
    char specialization[64];
    char contact[32];
} Doctor;

typedef struct {
    int id;
    int patient_id;
    int doctor_id;
    char date[20];
    char notes[128];
} Appointment;

typedef struct {
    int id;
    int appointment_id;
    double amount;
    char details[128];
    char date[20];
} Bill;

/* Utility: next id */
int next_id(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 1;

    int max = 0;
    if (!strcmp(filename, PAT_FILE)) {
        Patient p;
        while (fread(&p, sizeof(Patient), 1, f) == 1)
            if (p.id > max) max = p.id;
    }
    else if (!strcmp(filename, DOC_FILE)) {
        Doctor d;
        while (fread(&d, sizeof(Doctor), 1, f) == 1)
            if (d.id > max) max = d.id;
    }
    else if (!strcmp(filename, APPT_FILE)) {
        Appointment a;
        while (fread(&a, sizeof(Appointment), 1, f) == 1)
            if (a.id > max) max = a.id;
    }
    else if (!strcmp(filename, BILL_FILE)) {
        Bill b;
        while (fread(&b, sizeof(Bill), 1, f) == 1)
            if (b.id > max) max = b.id;
    }

    fclose(f);
    return max + 1;
}

/* Add Patient */
void add_patient() {
    centerText("=== ADD NEW PATIENT ===");

    Patient p;
    p.id = next_id(PAT_FILE);

    printf("Patient ID: %d\n", p.id);
    printf("Name: "); fgets(p.name, MAX_NAME, stdin); p.name[strcspn(p.name, "\n")] = 0;
    printf("Age: "); scanf("%d", &p.age); getchar();
    printf("Gender: "); fgets(p.gender, 16, stdin); p.gender[strcspn(p.gender, "\n")] = 0;
    printf("Contact: "); fgets(p.contact, 32, stdin); p.contact[strcspn(p.contact, "\n")] = 0;

    FILE *f = fopen(PAT_FILE, "ab");
    fwrite(&p, sizeof(Patient), 1, f);
    fclose(f);

    centerText("Patient Saved Successfully!");
}

/* List Patients */
void list_patients() {
    centerText("=== PATIENT LIST ===");

    FILE *f = fopen(PAT_FILE, "rb");
    if (!f) { centerText("No Patients Found."); return; }

    Patient p;
    printf("ID\tName\tAge\tGender\tContact\n");

    while (fread(&p, sizeof(Patient), 1, f) == 1)
        printf("%d\t%s\t%d\t%s\t%s\n", p.id, p.name, p.age, p.gender, p.contact);

    fclose(f);
}

/* Add Doctor */
void add_doctor() {
    centerText("=== ADD NEW DOCTOR ===");

    Doctor d;
    d.id = next_id(DOC_FILE);

    printf("Doctor ID: %d\n", d.id);
    printf("Name: "); fgets(d.name, MAX_NAME, stdin); d.name[strcspn(d.name, "\n")] = 0;
    printf("Specialization: "); fgets(d.specialization, 64, stdin); d.specialization[strcspn(d.specialization, "\n")] = 0;
    printf("Contact: "); fgets(d.contact, 32, stdin); d.contact[strcspn(d.contact, "\n")] = 0;

    FILE *f = fopen(DOC_FILE, "ab");
    fwrite(&d, sizeof(Doctor), 1, f);
    fclose(f);

    centerText("Doctor Saved Successfully!");
}

/* List Doctors */
void list_doctors() {
    centerText("=== DOCTOR LIST ===");

    FILE *f = fopen(DOC_FILE, "rb");
    if (!f) { centerText("No Doctors Found."); return; }

    Doctor d;
    printf("ID\tName\tSpecialization\tContact\n");

    while (fread(&d, sizeof(Doctor), 1, f) == 1)
        printf("%d\t%s\t%s\t%s\n", d.id, d.name, d.specialization, d.contact);

    fclose(f);
}

/* Helpers */
int patient_exists(int pid) {
    FILE *f = fopen(PAT_FILE, "rb");
    if (!f) return 0;
    Patient p;
    while (fread(&p, sizeof(Patient), 1, f) == 1)
        if (p.id == pid) { fclose(f); return 1; }
    fclose(f);
    return 0;
}

int doctor_exists(int did) {
    FILE *f = fopen(DOC_FILE, "rb");
    if (!f) return 0;
    Doctor d;
    while (fread(&d, sizeof(Doctor), 1, f) == 1)
        if (d.id == did) { fclose(f); return 1; }
    fclose(f);
    return 0;
}

/* Create Appointment */
void create_appointment() {
    centerText("=== CREATE APPOINTMENT ===");

    Appointment a;
    a.id = next_id(APPT_FILE);

    printf("Patient ID: ");
    scanf("%d", &a.patient_id); getchar();
    if (!patient_exists(a.patient_id)) { centerText("Patient Not Found."); return; }

    printf("Doctor ID: ");
    scanf("%d", &a.doctor_id); getchar();
    if (!doctor_exists(a.doctor_id)) { centerText("Doctor Not Found."); return; }

    printf("Date (YYYY-MM-DD HH:MM): ");
    fgets(a.date, 20, stdin); a.date[strcspn(a.date, "\n")] = 0;

    printf("Notes: ");
    fgets(a.notes, 128, stdin); a.notes[strcspn(a.notes, "\n")] = 0;

    FILE *f = fopen(APPT_FILE, "ab");
    fwrite(&a, sizeof(Appointment), 1, f);
    fclose(f);

    centerText("Appointment Saved Successfully!");
}

/* List Appointments */
void list_appointments() {
    centerText("=== APPOINTMENT LIST ===");

    FILE *f = fopen(APPT_FILE, "rb");
    if (!f) { centerText("No Appointments Found."); return; }

    Appointment a;
    printf("ID\tPatient\tDoctor\tDate\t\tNotes\n");

    while (fread(&a, sizeof(Appointment), 1, f) == 1)
        printf("%d\t%d\t%d\t%s\t%s\n", a.id, a.patient_id, a.doctor_id, a.date, a.notes);

    fclose(f);
}

/* Generate Bill */
void generate_bill() {
    centerText("=== GENERATE BILL ===");

    int appt_id;
    printf("Appointment ID: ");
    scanf("%d", &appt_id); getchar();

    FILE *f = fopen(APPT_FILE, "rb");
    if (!f) { centerText("No Appointments Found."); return; }

    Appointment a;
    int found = 0;
    while (fread(&a, sizeof(Appointment), 1, f) == 1)
        if (a.id == appt_id) { found = 1; break; }
    fclose(f);

    if (!found) { centerText("Appointment Not Found."); return; }

    Bill b;
    b.id = next_id(BILL_FILE);
    b.appointment_id = appt_id;

    printf("Amount: ");
    scanf("%lf", &b.amount); getchar();

    printf("Details: ");
    fgets(b.details, 128, stdin); b.details[strcspn(b.details, "\n")] = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(b.date, sizeof(b.date), "%04d-%02d-%02d %02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min);

    FILE *bf = fopen(BILL_FILE, "ab");
    fwrite(&b, sizeof(Bill), 1, bf);
    fclose(bf);

    centerText("Bill Generated Successfully!");
}

/* List Bills */
void list_bills() {
    centerText("=== BILL LIST ===");

    FILE *f = fopen(BILL_FILE, "rb");
    if (!f) { centerText("No Bills Found."); return; }

    Bill b;
    printf("ID\tAppointment\tAmount\tDate\t\tDetails\n");

    while (fread(&b, sizeof(Bill), 1, f) == 1)
        printf("%d\t%d\t\t%.2f\t%s\t%s\n", b.id, b.appointment_id, b.amount, b.date, b.details);

    fclose(f);
}

/* MENU */
void menu() {
    while (1) {
        centerText("=== SHORT HMS SYSTEM ===");
        centerText("1. Add Patient");
        centerText("2. List Patients");
        centerText("3. Add Doctor");
        centerText("4. List Doctors");
        centerText("5. Create Appointment");
        centerText("6. List Appointments");
        centerText("7. Generate Bill");
        centerText("8. List Bills");
        centerText("0. Exit");

        printf("\nChoose option: ");
        int opt;
        scanf("%d", &opt);
        getchar();

        switch (opt) {
            case 1: add_patient(); break;
            case 2: list_patients(); break;
            case 3: add_doctor(); break;
            case 4: list_doctors(); break;
            case 5: create_appointment(); break;
            case 6: list_appointments(); break;
            case 7: generate_bill(); break;
            case 8: list_bills(); break;
            case 0: centerText("Thank you! Exiting..."); return;
            default: centerText("Invalid Option! Try Again.");
        }
    }
}

int main() {
    centerText("Short Hospital Management System (HMS)");
    printf("\n");
    menu();
    return 0;
}
