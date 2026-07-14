#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MEETINGS 50
#define MAX_TITLE_LEN 100

typedef struct {
    int hour;
    int minute;
} Time;

typedef struct {
    Time start;
    Time end;
    char title[MAX_TITLE_LEN];
    int duration;
} Meeting;

Time start = {9, 0};    // Available from 9:00 AM
Time end = {17, 0};     // Available until 5:00 PM
Meeting meetings[MAX_MEETINGS];
int meeting_count = 0;

int time_to_minutes(Time t) {
    return t.hour * 60 + t.minute;
}

void minutes_to_time(int mins, Time *t) {
    t->hour = mins / 60;
    t->minute = mins % 60;
}

void display_time(Time t) {
    printf("%d:%02d %s", 
           (t.hour > 12) ? t.hour - 12 : (t.hour == 0 ? 12 : t.hour),
           t.minute,
           (t.hour >= 12) ? "PM" : "AM");
}

int is_time_valid(Time t) {
    return (t.hour >= 0 && t.hour < 24 && t.minute >= 0 && t.minute < 60);
}

int is_within_work_hours(Time t) {
    int mins = time_to_minutes(t);
    return (mins >= time_to_minutes(start) && mins <= time_to_minutes(end));
}

int is_slot_available(Time proposed_start, int duration) {
    int proposed_start_mins = time_to_minutes(proposed_start);
    int proposed_end_mins = proposed_start_mins + duration;
    
    // Check if within work hours
    if (proposed_end_mins > time_to_minutes(end)) {
        return 0;
    }
    
    // Check against existing meetings
    for (int i = 0; i < meeting_count; i++) {
        int meeting_start = time_to_minutes(meetings[i].start);
        int meeting_end = meeting_start + meetings[i].duration;
        
        // Check for overlap
        if (!(proposed_end_mins <= meeting_start || proposed_start_mins >= meeting_end)) {
            return 0;
        }
    }
    return 1;
}

void suggest_meeting() {
    int duration;
    char title[MAX_TITLE_LEN];
    
    printf("Enter meeting title: ");
    getchar(); // Clear newline
    fgets(title, MAX_TITLE_LEN, stdin);
    title[strcspn(title, "\n")] = 0; // Remove newline
    
    printf("Enter meeting duration (minutes): ");
    scanf("%d", &duration);
    
    if (duration <= 0) {
        printf("Error: Duration must be positive!\n");
        return;
    }
    
    // Find earliest available slot
    int current_mins = time_to_minutes(start);
    int found_slot = 0;
    Time suggested_start;
    
    while (current_mins + duration <= time_to_minutes(end)) {
        minutes_to_time(current_mins, &suggested_start);
        if (is_slot_available(suggested_start, duration)) {
            found_slot = 1;
            break;
        }
        current_mins += 5; // Check in 5-minute increments
    }
    
    if (!found_slot) {
        printf("No available slot for %d minutes today!\n", duration);
        return;
    }
    
    // Schedule the meeting
    Meeting new_meeting;
    new_meeting.start = suggested_start;
    new_meeting.duration = duration;
    minutes_to_time(time_to_minutes(suggested_start) + duration, &new_meeting.end);
    strcpy(new_meeting.title, title);
    
    meetings[meeting_count++] = new_meeting;
    
    printf("\n✓ Meeting scheduled!\n");
    printf("Title: %s\n", new_meeting.title);
    printf("Time: ");
    display_time(new_meeting.start);
    printf(" to ");
    display_time(new_meeting.end);
    printf(" (%d minutes)\n", duration);
}

void display_schedule() {
    printf("\n========== SCHEDULE ==========\n");
    if (meeting_count == 0) {
        printf("No meetings scheduled today.\n");
        return;
    }
    
    // Sort meetings by start time
    for (int i = 0; i < meeting_count - 1; i++) {
        for (int j = i + 1; j < meeting_count; j++) {
            if (time_to_minutes(meetings[i].start) > time_to_minutes(meetings[j].start)) {
                Meeting temp = meetings[i];
                meetings[i] = meetings[j];
                meetings[j] = temp;
            }
        }
    }
    
    printf("%-20s %-20s %-10s\n", "Title", "Time", "Duration");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < meeting_count; i++) {
        printf("%-20s ", meetings[i].title);
        display_time(meetings[i].start);
        printf(" - ");
        display_time(meetings[i].end);
        printf(" %d min\n", meetings[i].duration);
    }
    
    // Show available slots
    printf("\nAvailable slots:\n");
    int current_mins = time_to_minutes(start);
    int found_available = 0;
    
    for (int i = 0; i < meeting_count; i++) {
        int meeting_start = time_to_minutes(meetings[i].start);
        if (meeting_start > current_mins) {
            Time slot_start, slot_end;
            minutes_to_time(current_mins, &slot_start);
            minutes_to_time(meeting_start, &slot_end);
            printf("  ");
            display_time(slot_start);
            printf(" - ");
            display_time(slot_end);
            printf(" (%d min)\n", meeting_start - current_mins);
            found_available = 1;
        }
        current_mins = meeting_start + meetings[i].duration;
    }
    
    if (current_mins < time_to_minutes(end)) {
        Time slot_start, slot_end;
        minutes_to_time(current_mins, &slot_start);
        minutes_to_time(time_to_minutes(end), &slot_end);
        printf("  ");
        display_time(slot_start);
        printf(" - ");
        display_time(slot_end);
        printf(" (%d min)\n", time_to_minutes(end) - current_mins);
        found_available = 1;
    }
    
    if (!found_available) {
        printf("  No available slots\n");
    }
}

void clear_schedule() {
    char confirm;
    printf("Are you sure you want to clear all meetings? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm == 'y' || confirm == 'Y') {
        meeting_count = 0;
        printf("Schedule cleared!\n");
    } else {
        printf("Operation cancelled.\n");
    }
}

void edit_meeting() {
    if (meeting_count == 0) {
        printf("No meetings to edit.\n");
        return;
    }
    
    display_schedule();
    int index;
    printf("\nEnter meeting number to edit (1-%d): ", meeting_count);
    scanf("%d", &index);
    
    if (index < 1 || index > meeting_count) {
        printf("Invalid meeting number!\n");
        return;
    }
    
    index--; // Convert to 0-based
    
    printf("Current title: %s\n", meetings[index].title);
    printf("Enter new title: ");
    getchar();
    fgets(meetings[index].title, MAX_TITLE_LEN, stdin);
    meetings[index].title[strcspn(meetings[index].title, "\n")] = 0;
    
    printf("Enter new duration (minutes): ");
    int new_duration;
    scanf("%d", &new_duration);
    
    if (new_duration > 0) {
        int old_duration = meetings[index].duration;
        meetings[index].duration = new_duration;
        minutes_to_time(time_to_minutes(meetings[index].start) + new_duration, 
                       &meetings[index].end);
        printf("Meeting updated successfully!\n");
    } else {
        printf("Duration unchanged.\n");
    }
}

void view_work_hours() {
    printf("\nWork hours: ");
    display_time(start);
    printf(" - ");
    display_time(end);
    printf("\nTotal available: %d minutes\n", 
           time_to_minutes(end) - time_to_minutes(start));
    
    if (meeting_count > 0) {
        int total_scheduled = 0;
        for (int i = 0; i < meeting_count; i++) {
            total_scheduled += meetings[i].duration;
        }
        printf("Total scheduled: %d minutes\n", total_scheduled);
        printf("Remaining time: %d minutes (%.1f%%)\n", 
               time_to_minutes(end) - time_to_minutes(start) - total_scheduled,
               (float)(time_to_minutes(end) - time_to_minutes(start) - total_scheduled) / 
               (time_to_minutes(end) - time_to_minutes(start)) * 100);
    }
}

int main() {
    printf("==================================\n");
    printf("   MEETING SCHEDULER v2.0\n");
    printf("==================================\n");
    printf("Work hours: ");
    display_time(start);
    printf(" - ");
    display_time(end);
    printf("\n");
    
    int choice;
    do {
        printf("\n+----------------------------------+\n");
        printf("| 1. Schedule Meeting               |\n");
        printf("| 2. View Schedule                  |\n");
        printf("| 3. Clear All Meetings             |\n");
        printf("| 4. Edit Meeting                   |\n");
        printf("| 5. View Work Hours & Statistics   |\n");
        printf("| 6. Exit                           |\n");
        printf("+----------------------------------+\n");
        printf("Choice: ");
        
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                suggest_meeting();
                break;
            case 2:
                display_schedule();
                break;
            case 3:
                clear_schedule();
                break;
            case 4:
                edit_meeting();
                break;
            case 5:
                view_work_hours();
                break;
            case 6:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 6);
    
    return 0;
}
