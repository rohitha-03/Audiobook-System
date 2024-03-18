#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 100

typedef struct Preference_tag
{
    char pref[MAX_SIZE];
    struct Preference_tag *prefnext;
} Preference;

typedef struct Timestamp_tag
{
    int h;
    int m;
    int s;
} Timestamp;

typedef struct audio_tag
{
    int AudiobookID;
    char Title[MAX_SIZE];
    char Author[MAX_SIZE];
    char Narrator[MAX_SIZE];
    int hours;
    int mins;
    int sec;
    char genre[MAX_SIZE];
    float rating;
    struct audio_tag *audionext;
} Audio;

typedef struct userAudio_tag
{
    int userA_ID;
    Timestamp timestamp;
    float rating;
    struct userAudio_tag *userA_next;
} userAudio;

typedef struct user_tag
{
    int UserID;
    char Name[MAX_SIZE];
    char Email[MAX_SIZE];
    Preference *preference;
    struct user_tag *usernext;
    userAudio *audiolist;
} user;

Audio *Mainlist;
user *Userlist;

void display(user *);

Audio *CreateNode(int ID, char title[], char author[], char narrator[], int hrs, int min, int s, char genre[], float rating)
{
    // printf("\nCreateNode");
    Audio *nptr = (Audio *)malloc(sizeof(Audio));
    nptr->AudiobookID = ID;
    strcpy(nptr->Title, title);
    strcpy(nptr->Author, author);
    strcpy(nptr->Narrator, narrator);
    nptr->hours = hrs;
    nptr->mins = min;
    nptr->sec = s;
    strcpy(nptr->genre, genre);
    nptr->rating = rating;
    nptr->audionext = NULL;

    return nptr;
}

Audio *mergesort_AudioID(Audio *);

void CreateMainList()
{
    Mainlist = NULL;
    FILE *fptr = fopen("Audiobook.csv", "r");
    if (fptr == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    // printf("Inside create\n");

    char line[MAX_SIZE];
    int AudiobookID;
    char Title[MAX_SIZE], author[MAX_SIZE], narrator[MAX_SIZE], genre[MAX_SIZE];
    int hrs, mins, s;
    float rating;
    // char line[MAX_SIZE];
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%d:%d:%d,%[^,],%f", &AudiobookID, Title, author, narrator, &hrs, &mins, &s, genre, &rating);
        Audio *nptr = CreateNode(AudiobookID, Title, author, narrator, hrs, mins, s, genre, rating);
        nptr->audionext = Mainlist;
        Mainlist = nptr;
    }
    fclose(fptr);
    Mainlist = mergesort_AudioID(Mainlist);
}

Audio *merge(Audio *lptr1, Audio *lptr2)
{
    Audio *ptr1, *ptr2, *result, *tail;
    if (lptr2 == NULL)
    {
        return lptr1;
    }
    else if (lptr1 == NULL)
    {
        return lptr2;
    }
    else
    {
        ptr1 = lptr1;
        ptr2 = lptr2;
        if (ptr1->AudiobookID < ptr2->AudiobookID)
        {
            result = lptr1;
            ptr1 = ptr1->audionext;
        }
        else
        {
            result = lptr2;
            ptr2 = ptr2->audionext;
        }
        tail = result;
        while (ptr1 != NULL && ptr2 != NULL)
        {
            if (ptr1->AudiobookID < ptr2->AudiobookID)
            {
                tail->audionext = ptr1;
                tail = tail->audionext;
                ptr1 = ptr1->audionext;
            }
            else
            {
                tail->audionext = ptr2;
                tail = tail->audionext;
                ptr2 = ptr2->audionext;
            }
        }
        if (ptr1 != NULL)
        {
            tail->audionext = ptr1;
        }
        if (ptr2 != NULL)
        {
            tail->audionext = ptr2;
        }
    }
    return result;
}

Audio *Divide_Audio(Audio *lptr)
{
    Audio *fast, *slow, *ptr;
    slow = lptr;
    fast = lptr->audionext->audionext;
    while (fast != NULL)
    {
        slow = slow->audionext;
        fast = fast->audionext;
        if (fast != NULL)
        {
            fast = fast->audionext;
        }
    }
    ptr = slow->audionext;
    slow->audionext = NULL;
    return ptr;
}

Audio *mergesort_AudioID(Audio *lptr)
{
    Audio *ptr;
    Audio *head = lptr;
    if ((head != NULL) && (head->audionext != NULL))
    {
        ptr = Divide_Audio(lptr);
        head = mergesort_AudioID(head);
        ptr = mergesort_AudioID(ptr);
        lptr = merge(head, ptr);
    }
    return lptr;
}

void displaylist()
{
    Audio *ptr;
    ptr = Mainlist;
    while (ptr != NULL)
    {
        printf("id=%d\n", ptr->AudiobookID);
        printf("title=%s\n", ptr->Title);
        printf("author=%s\n", ptr->Author);
        printf("narrator=%s\n", ptr->Narrator);
        printf("duration=%d:%d:%d\n", ptr->hours, ptr->mins, ptr->sec);
        printf("genre=%s\n", ptr->genre);
        printf("rating=%f\n", ptr->rating);
        ptr = ptr->audionext;
    }
}

void Add_Audiobook()
{
    printf("Enter details of the audiobook\n");
    int id, hrs, mins, s;
    char title[MAX_SIZE], author[MAX_SIZE], narrator[MAX_SIZE], genre[MAX_SIZE];
    float rating;
    Audio *ptr = Mainlist, *prev = NULL;

    printf("Enter ID:");
    scanf("%d", &id);
    printf("\nEnter Title:");
    scanf(" %[^\n]", title);
    printf("\nEnter author:");
    scanf(" %[^\n]", author);
    printf("\nEnter narrator:");
    scanf(" %[^\n]", narrator);
    printf("\nEnter duration in terms of hrs,mins and sec:");
    scanf("%d%d%d", &hrs, &mins, &s);
    printf("\nEnter genre:");
    scanf(" %[^\n]", genre);
    printf("\nEnter rating:");
    scanf("%f", &rating);

    Audio *nptr = CreateNode(id, title, author, narrator, hrs, mins, s, genre, rating);

    if (Mainlist == NULL || Mainlist->AudiobookID > nptr->AudiobookID)
    {
        nptr->audionext = Mainlist;
        Mainlist = nptr;
    }
    else
    {
        // printf("\nelse");
        while (ptr != NULL && ptr->AudiobookID < nptr->AudiobookID)
        {
            prev = ptr;
            ptr = ptr->audionext;
        }
        nptr->audionext = ptr;
        prev->audionext = nptr;
    }

    printf("\nThe audiobook has been successfully added");
    displaylist();
}

void Edit_Audiobook()
{
    int id;
    printf("enter the audiobook id that you want to edit:");
    scanf("%d", &id);
    printf("enter the field you want to edit:\n1.Title\n2.Author\n3.Narrator\n4.Duration\n5.genre\nEnter choice:");
    int choice;
    scanf("%d", &choice);

    FILE *fp = fopen("Audiobook1.csv", "w");

    Audio *ptr;
    ptr = Mainlist;
    while (ptr->AudiobookID != id)
    {
        ptr = ptr->audionext;
    }
    if (choice != 4)
    {
        printf("enter the new field data:");
        char str[MAX_SIZE];
        scanf(" %[^\n]", str);
        if (choice == 1)
        {
            strcpy(ptr->Title, str);
        }
        else if (choice == 2)
        {
            strcpy(ptr->Author, str);
        }
        else if (choice == 3)
        {
            strcpy(ptr->Narrator, str);
        }
        else if (choice == 5)
        {
            strcpy(ptr->genre, str);
        }
    }
    else
    {
        int h, m, s;
        printf("Enter the duration:");
        scanf("%d%d%d", &h, &m, &s);
        ptr->hours = h;
        ptr->mins = m;
        ptr->sec = s;
    }
    printf("\nThe audiobook has been successfully modified");
    displaylist();
}

void Delete_Audiobook()
{
    printf("enter the Audiobook Title or Author you want to delete:");
    char str[MAX_SIZE];
    scanf(" %[^\n]", str);
    Audio *prev, *ptr;
    int id = -1;
    prev = NULL;
    ptr = Mainlist;
    while (ptr != NULL)
    {
        if (strcmp(ptr->Title, str) == 0 || strcmp(ptr->Author, str) == 0)
        {
            id = ptr->AudiobookID;
            if (prev == NULL)
            {
                Mainlist = Mainlist->audionext;
                free(ptr);
            }
            else
            {
                prev->audionext = ptr->audionext;
                free(ptr);
            }
            ptr = NULL;
        }
        else
        {
            prev = ptr;
            ptr = ptr->audionext;
        }
    }
    if (prev->audionext == NULL)
    {
        printf("No such Audiobook found\n");
    }
    else
    {
        printf("check1");
        user *nptr = Userlist;

        while (nptr != NULL)
        {
            printf("check");
            userAudio *aptr = nptr->audiolist;
            userAudio *prev = NULL;
            int flag = 0;
            while (aptr != NULL && flag == 0)
            {
                if (aptr->userA_ID == id)
                {
                    if (prev == NULL && aptr != NULL)
                    {
                        nptr->audiolist = aptr->userA_next;
                        free(aptr);
                    }
                    else
                    {
                        prev->userA_next = aptr->userA_next;
                        free(aptr);
                    }
                    flag = 1;
                }
                else
                {
                    prev = aptr;
                    aptr = aptr->userA_next;
                }
            }
            nptr = nptr->usernext;
        }
    }
    if (id != -1)
    {
        printf("The audiobook has been deleted\n");
    }
    displaylist();
}

void Search_Audiobook()
{
    printf("Enter title,author or genre which you want to search:");
    char str[MAX_SIZE];
    scanf(" %[^\n]", str);
    Audio *ptr = Mainlist;

    while (ptr != NULL)
    {
        if ((strcmp(ptr->Title, str) == 0) || (strcmp(ptr->Author, str) == 0 || (strcmp(ptr->genre, str) == 0)))
        {
            printf("id=%d\n", ptr->AudiobookID);
            printf("title=%s\n", ptr->Title);
            printf("\n");
        }
        ptr = ptr->audionext;
    }
}

user *Add_user(user *ul, user *nptr)
{
    if (ul == NULL)
    {
        ul = nptr;
    }
    else
    {
        user *ptr, *prev;
        ptr = ul;
        prev = NULL;
        while (ptr != NULL && ptr->UserID < nptr->UserID)
        {
            prev = ptr;
            ptr = ptr->usernext;
        }
        if (prev == NULL)
        {
            nptr->usernext = ptr;
            ul = nptr;
        }
        else
        {
            prev->usernext = nptr;
            nptr->usernext = ptr;
        }
    }
    return ul;
    display(Userlist);
}

userAudio *Add_userAudio(userAudio *ual, userAudio *temp)
{
    if (ual == NULL)
    {
        ual = temp;
    }
    else
    {
        userAudio *ptr = ual;
        userAudio *prev = NULL;
        while (ptr != NULL && ptr->userA_ID < temp->userA_ID)
        {
            prev = ptr;
            ptr = ptr->userA_next;
        }
        if (prev == NULL)
        {
            temp->userA_next = ptr;
            ual = temp;
        }
        else
        {
            prev->userA_next = temp;
            temp->userA_next = ptr;
        }
    }
    return ual;
}

userAudio *merge_UaudioID(userAudio *lptr1, userAudio *lptr2)
{
    userAudio *ptr1, *ptr2, *result, *tail;
    if (lptr2 == NULL)
    {
        return lptr1;
    }
    else if (lptr1 == NULL)
    {
        return lptr2;
    }
    else
    {
        ptr1 = lptr1;
        ptr2 = lptr2;
        if (lptr1->userA_ID < lptr2->userA_ID)
        {
            result = lptr1;
            ptr1 = ptr1->userA_next;
        }
        else
        {
            result = lptr2;
            ptr2 = ptr2->userA_next;
        }
        tail = result;
        while (ptr1 != NULL && ptr2 != NULL)
        {
            if (ptr1->userA_ID < ptr2->userA_ID)
            {
                tail->userA_next = ptr1;
                tail = tail->userA_next;
                ptr1 = ptr1->userA_next;
            }
            else
            {
                tail->userA_next = ptr2;
                tail = tail->userA_next;
                ptr2 = ptr2->userA_next;
            }
        }
        if (ptr1 != NULL)
        {
            tail->userA_next = ptr1;
        }
        if (ptr2 != NULL)
        {
            tail->userA_next = ptr2;
        }
    }
    return result;
}

userAudio *Divide(userAudio *lptr)
{
    userAudio *fast, *slow, *ptr;
    slow = lptr;
    fast = lptr->userA_next->userA_next;
    while (fast != NULL)
    {
        slow = slow->userA_next;
        fast = fast->userA_next;
        if (fast != NULL)
        {
            fast = fast->userA_next;
        }
    }
    ptr = slow->userA_next;
    slow->userA_next = NULL;
    return ptr;
}

userAudio *mergesort_UaudioID(userAudio *lp)
{
    userAudio *ptr;
    userAudio *head = lp;
    if ((head != NULL) && (head->userA_next != NULL))
    {
        ptr = Divide(lp);
        head = mergesort_UaudioID(head);
        ptr = mergesort_UaudioID(ptr);
        lp = merge_UaudioID(head, ptr);
    }
    return lp;
}

user *Merge_Userlist(user *lptr1, user *lptr2)
{
    user *ptr1, *ptr2, *result, *tail;
    if (lptr2 == NULL)
    {
        return lptr1;
    }
    else if (lptr1 == NULL)
    {
        return lptr2;
    }
    else
    {
        ptr1 = lptr1;
        ptr2 = lptr2;
        if (lptr1->UserID < lptr2->UserID)
        {
            result = lptr1;
            ptr1 = ptr1->usernext;
        }
        else
        {
            result = lptr2;
            ptr2 = ptr2->usernext;
        }
        tail = result;
        while (ptr1 != NULL && ptr2 != NULL)
        {
            if (ptr1->UserID < ptr2->UserID)
            {
                tail->usernext = ptr1;
                tail = tail->usernext;
                ptr1 = ptr1->usernext;
            }
            else
            {
                tail->usernext = ptr2;
                tail = tail->usernext;
                ptr2 = ptr2->usernext;
            }
        }
        if (ptr1 != NULL)
        {
            tail->usernext = ptr1;
        }
        if (ptr2 != NULL)
        {
            tail->usernext = ptr2;
        }
    }
    return result;
}

user *Divide_user(user *lptr)
{
    user *fast, *slow, *ptr;
    slow = lptr;
    fast = lptr->usernext->usernext;
    while (fast != NULL)
    {
        slow = slow->usernext;
        fast = fast->usernext;
        if (fast != NULL)
        {
            fast = fast->usernext;
        }
    }
    ptr = slow->usernext;
    slow->usernext = NULL;
    return ptr;
}

user *mergesort_Userlist(user *lp)
{
    user *ptr;
    user *head = lp;
    if ((head != NULL) && (head->usernext != NULL))
    {
        ptr = Divide_user(lp);
        head = mergesort_Userlist(head);
        ptr = mergesort_Userlist(ptr);
        lp = Merge_Userlist(head, ptr);
    }
    return lp;
}

void CreateUserList()
{
    FILE *fptr = fopen("Userlist.txt", "r");
    char line[MAX_SIZE];
    Userlist = NULL;
    user *usertemp = NULL;
    if (fptr == NULL)
    {
        printf("unable to open file\n");
    }
    else
    {
        int case_no = 0;
        while (fgets(line, sizeof(line), fptr) != NULL)
        {
            if (strncmp(line, "/", 1) == 0)
            {
                usertemp = (user *)malloc(sizeof(user));
                usertemp->preference = NULL;
                usertemp->audiolist = NULL;
                usertemp->usernext = NULL;

                case_no = 1;
            }
            else if (strncmp(line, "#", 1) == 0)
            {
                // Userlist = Add_user(Userlist, usertemp);
                usertemp->usernext = Userlist;
                Userlist = usertemp;
                usertemp->audiolist = mergesort_UaudioID(usertemp->audiolist);
            }
            else
            {
                if (strncmp(line, "Pref", 4) == 0)
                {
                    case_no = 4;
                }
                else if (strncmp(line, "Audio", 5) == 0)
                {
                    case_no = 5;
                }
                else
                {
                    switch (case_no)
                    {
                    case 1:
                    {
                        usertemp->UserID = atoi(line);
                        case_no++;
                        break;
                    }
                    case 2:
                    {
                        strcpy(usertemp->Name, line);
                        case_no++;
                        break;
                    }
                    case 3:
                    {
                        strcpy(usertemp->Email, line);
                        break;
                    }
                    case 4:
                    {
                        Preference *temppref = (Preference *)malloc(sizeof(Preference));
                        strcpy(temppref->pref, line);
                        temppref->prefnext = NULL;
                        if (usertemp->preference == NULL)
                        {
                            usertemp->preference = temppref;
                        }
                        else
                        {
                            Preference *ptr = usertemp->preference;
                            while (ptr->prefnext != NULL)
                            {
                                ptr = ptr->prefnext;
                            }
                            ptr->prefnext = temppref;
                        }
                        break;
                    }
                    case 5:
                    {
                        userAudio *nptr = (userAudio *)malloc(sizeof(userAudio));
                        nptr->userA_ID = atoi(line);
                        nptr->rating = 0;
                        nptr->timestamp.h = 0;
                        nptr->timestamp.m = 0;
                        nptr->timestamp.s = 0;
                        // nptr->userA_next = NULL;
                        // usertemp->audiolist = Add_userAudio(usertemp->audiolist, nptr);
                        nptr->userA_next = usertemp->audiolist;
                        usertemp->audiolist = nptr;
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }
    }
    fclose(fptr);
    Userlist = mergesort_Userlist(Userlist);
}

void display(user *list)
{
    user *ptr;
    ptr = list;

    while (ptr != NULL)
    {
        printf("%d\n", ptr->UserID);
        printf("%s", ptr->Name);
        printf("\n%s", ptr->Email);
        Preference *prefptr = ptr->preference;
        while (prefptr != NULL)
        {
            printf("\n%s", prefptr->pref);
            prefptr = prefptr->prefnext;
        }
        userAudio *audioptr = ptr->audiolist;
        while (audioptr != NULL)
        {
            printf("\n%d", audioptr->userA_ID);
            printf("\n%d:%d:%d", audioptr->timestamp.h, audioptr->timestamp.m, audioptr->timestamp.s);
            printf("\n%f", audioptr->rating);
            audioptr = audioptr->userA_next;
        }
        ptr = ptr->usernext;
    }
}

void delete_user_list(user *head)
{
    user *current = head;
    user *next;

    while (current != NULL)
    {
        // Free preference linked list
        Preference *pref_current = current->preference;
        Preference *pref_next;
        while (pref_current != NULL)
        {
            pref_next = pref_current->prefnext;
            free(pref_current);
            pref_current = pref_next;
        }

        // Free audio linked list
        userAudio *audio_current = current->audiolist;
        userAudio *audio_next;
        while (audio_current != NULL)
        {
            audio_next = audio_current->userA_next;
            free(audio_current);
            audio_current = audio_next;
        }

        next = current->usernext;
        free(current);
        current = next;
    }
}

Audio *return_audioptr_of_Main(int aid)
{
    Audio *ptr = Mainlist;
    while (ptr != NULL && ptr->AudiobookID != aid)
    {
        ptr = ptr->audionext;
    }
    return ptr;
}

user *return_userptr(int uid)
{
    user *ptr = Userlist;
    while (ptr != NULL && ptr->UserID != uid)
    {
        ptr = ptr->usernext;
    }
    return ptr;
}

user *Add_user_profile(user *Userlist)
{
    user *newuser = (user *)malloc(sizeof(user));
    int num;
    char preferences[MAX_SIZE];
    printf("Enter your name:");
    scanf(" %[^\n]", newuser->Name);
    printf("\nEnter your ID:");
    scanf("%d", &newuser->UserID);
    printf("\nEnter your Email:");
    scanf("%s", newuser->Email);
    newuser->preference = NULL;
    newuser->audiolist = NULL;
    newuser->usernext = NULL;
    printf("\nEnter the number of preferences:");
    scanf("%d", &num);
    // Preference *pref = (Preference *)malloc(sizeof(Preference));
    printf("\nEnter the preferences:");
    while (num--)
    {
        scanf(" %[^\n]", preferences);
        Preference *temp = (Preference *)malloc(sizeof(Preference));
        strcpy(temp->pref, preferences);
        temp->prefnext = NULL;
        if (newuser->preference == NULL)
        {
            newuser->preference = temp;
        }
        else
        {
            Preference *ptr = newuser->preference;
            while (ptr->prefnext != NULL)
            {
                ptr = ptr->prefnext;
            }
            ptr->prefnext = temp;
        }
    }
    Userlist = Add_user(Userlist, newuser);
    printf("The user account has been successfully created\n");
    display(Userlist);
    return Userlist;
}

user *Add_to_user_library(user *Userlist)
{
    int uid, n, aid;
    printf("Enter your User ID:");
    scanf("%d", &uid);
    printf("You have two options:\n1.Add an audiobook\n2.Remove an audiobook\nEnter your choice:");
    scanf("%d", &n);
    printf("Enter audioBook Id which you want to add or remove:");
    scanf("%d", &aid);
    user *u = Userlist;
    while (u != NULL && u->UserID != uid)
    {
        u = u->usernext;
    }
    switch (n)
    {
    case 1:
    {
        printf("Adding audiobook\n");
        userAudio *temp_book = (userAudio *)malloc(sizeof(userAudio));
        temp_book->userA_ID = aid;
        temp_book->userA_next = NULL;
        temp_book->rating = temp_book->timestamp.h = temp_book->timestamp.m = temp_book->timestamp.s = 0;
        u->audiolist = Add_userAudio(u->audiolist, temp_book);
        break;
    }
    case 2:
    {
        printf("Removing audiobook\n");
        userAudio *ptr, *prev;
        prev = NULL;
        ptr = u->audiolist;
        while (ptr != NULL && ptr->userA_ID != aid)
        {
            prev = ptr;
            ptr = ptr->userA_next;
        }
        if (ptr != NULL)
        {
            if (prev == NULL)
            {
                u->audiolist = ptr->userA_next;
            }
            else
            {
                prev->userA_next = ptr->userA_next;
            }
            free(ptr);
        }
        else
            printf("Audiobook not found\n");
        break;
    }
    default:
        break;
    }
    display(Userlist);
    return Userlist;
}

int compare_timestamp(Timestamp t1, Timestamp t2)
{
    int t;
    if (t1.h > t2.h)
    {
        t = 1;
    }
    else
    {
        if (t1.h == t2.h)
        {
            if (t1.m > t2.m)
            {
                t = 1;
            }
            else
            {
                if (t1.m == t2.m)
                {
                    if (t1.s > t2.s)
                    {
                        t = 1;
                    }
                    else
                    {
                        t = 0;
                    }
                }
                else
                {
                    t = 0;
                }
            }
        }
        else
        {
            t = 0;
        }
    }
    return t;
}

userAudio *merge_UaudioTimestamp(userAudio *lptr1, userAudio *lptr2)
{
    userAudio *ptr1, *ptr2, *result, *tail;
    if (lptr2 == NULL)
    {
        return lptr1;
    }
    else if (lptr1 == NULL)
    {
        return lptr2;
    }
    else
    {
        ptr1 = lptr1;
        ptr2 = lptr2;
        Timestamp t1 = lptr1->timestamp;
        Timestamp t2 = lptr2->timestamp;
        if (compare_timestamp(t1, t2))
        {
            result = lptr1;
            ptr1 = ptr1->userA_next;
        }
        else
        {
            result = lptr2;
            ptr2 = ptr2->userA_next;
        }
        tail = result;
        while (ptr1 != NULL && ptr2 != NULL)
        {
            if (compare_timestamp(ptr1->timestamp, ptr2->timestamp) == 1)
            {
                tail->userA_next = ptr1;
                tail = tail->userA_next;
                ptr1 = ptr1->userA_next;
            }
            else
            {
                tail->userA_next = ptr2;
                tail = tail->userA_next;
                ptr2 = ptr2->userA_next;
            }
        }
        if (ptr1 != NULL)
        {
            tail->userA_next = ptr1;
        }
        if (ptr2 != NULL)
        {
            tail->userA_next = ptr2;
        }
    }
    return result;
}

userAudio *mergesort_timestamp(userAudio *lp)
{
    userAudio *ptr;
    userAudio *head = lp;
    if ((head != NULL) && (head->userA_next != NULL))
    {
        ptr = Divide(lp);
        head = mergesort_timestamp(head);
        ptr = mergesort_timestamp(ptr);
        lp = merge_UaudioTimestamp(head, ptr);
    }
    return lp;
}

void ListeningProgress()
{
    int uid, aid, h, m, s;
    printf("Enter User ID");
    scanf("%d", &uid);
    printf("\nEnter Audiobook ID");
    scanf("%d", &aid);
    char time[10];
    printf("\nEnter listening progress in hh:mm:ss format");
    scanf("%s", time);
    user *ptr = return_userptr(uid);

    userAudio *audioptr = ptr->audiolist;
    while (audioptr != NULL && audioptr->userA_ID != aid)
    {
        audioptr = audioptr->userA_next;
    }

    sscanf(time, "%d:%d:%d", &h, &m, &s);
    audioptr->timestamp.h += h;
    audioptr->timestamp.m += m;
    audioptr->timestamp.s += s;
    if (audioptr->timestamp.s >= 60)
    {
        audioptr->timestamp.m += (audioptr->timestamp.s / 60);
        audioptr->timestamp.s = (audioptr->timestamp.s % 60);
    }
    if (audioptr->timestamp.m >= 60)
    {
        audioptr->timestamp.h += (audioptr->timestamp.m / 60);
        audioptr->timestamp.m = (audioptr->timestamp.m % 60);
    }
    printf("timestamp has been successfully updated to %d:%d:%d", audioptr->timestamp.h, audioptr->timestamp.m, audioptr->timestamp.s);
    display(Userlist);
}

void Display_User_Library()
{
    int uid;
    char str[MAX_SIZE];
    printf("Enter The User ID:");
    scanf("%d", &uid);
    printf("Enter The filter criteria:");
    scanf(" %[^\n]", str);
    user *uptr = return_userptr(uid);
    uptr->audiolist = mergesort_timestamp(uptr->audiolist);
    userAudio *ptr = uptr->audiolist;
    if (strcmp(str, "All Audiobooks") != 0)
    {
        while (ptr != NULL)
        {
            Audio *aptr = return_audioptr_of_Main(ptr->userA_ID);
            if ((strcmp(str, aptr->Author) == 0) || (strcmp(str, aptr->Title) == 0) || (strcmp(str, aptr->Narrator) == 0) || (strcmp(str, aptr->genre) == 0))
            {
                printf("Audiobook ID:%d\n", ptr->userA_ID);
            }
            ptr = ptr->userA_next;
        }

    }
    else
    {
        while (ptr != NULL)
        {
            printf("Audiobook ID:%d\n", ptr->userA_ID);
            ptr = ptr->userA_next;
        }
    }
    uptr->audiolist = mergesort_UaudioID(uptr->audiolist);
}

void set_AverageRating(Audio *nptr)
{
    user *uptr = Userlist;
    int count = 0;
    float sum = 0;
    while (uptr != NULL)
    {
        int flag = 0;
        userAudio *aptr = uptr->audiolist;
        while (aptr != NULL && flag == 0)
        {
            if (aptr->userA_ID == nptr->AudiobookID)
            {
                if (aptr->rating != 0.0)
                {
                    sum = sum + (aptr->rating);
                    count++;
                }
                flag = 1;
            }
            aptr = aptr->userA_next;
        }
        uptr = uptr->usernext;
    }
    printf("%d ", count);
    if (count != 0)
    {
        nptr->rating = (sum / count);
    }
}

void Rate_Audiobook()
{
    int uid, aid;
    float rate;
    printf("Enter userid:");
    scanf("%d", &uid);
    printf("Enter Audiobookid:");
    scanf("%d", &aid);
    printf("Enter th rating you want to give for the Audiobook(rate between 1-5):");
    scanf("%f", &rate);
    user *temp = return_userptr(uid);
    userAudio *ptr = temp->audiolist;
    int flag = 0;
    while (ptr != NULL && flag == 0)
    {
        if (ptr->userA_ID == aid)
        {
            ptr->rating = rate;
            flag = 1;
        }
        ptr = ptr->userA_next;
    }
    Audio *nptr = return_audioptr_of_Main(aid);
    set_AverageRating(nptr);
    printf("Rating has been successfully recorded\n");
}

Audio *merge_Audio(Audio *lptr1, Audio *lptr2)
{
    Audio *ptr1, *ptr2, *result, *tail;
    if (lptr2 == NULL)
    {
        return lptr1;
    }
    else if (lptr1 == NULL)
    {
        return lptr2;
    }
    else
    {
        ptr1 = lptr1;
        ptr2 = lptr2;
        if (ptr1->rating > ptr2->rating)
        {
            result = lptr1;
            ptr1 = ptr1->audionext;
        }
        else
        {
            result = lptr2;
            ptr2 = ptr2->audionext;
        }
        tail = result;
        while (ptr1 != NULL && ptr2 != NULL)
        {
            if (ptr1->rating > ptr2->rating)
            {
                tail->audionext = ptr1;
                tail = tail->audionext;
                ptr1 = ptr1->audionext;
            }
            else
            {
                tail->audionext = ptr2;
                tail = tail->audionext;
                ptr2 = ptr2->audionext;
            }
        }
        if (ptr1 != NULL)
        {
            tail->audionext = ptr1;
        }
        if (ptr2 != NULL)
        {
            tail->audionext = ptr2;
        }
    }
    return result;
}

Audio *mergesort_avgrating(Audio *lp)
{
    Audio *ptr;
    Audio *head = lp;
    if ((head != NULL) && (head->audionext != NULL))
    {
        ptr = Divide_Audio(lp);
        head = mergesort_avgrating(head);
        ptr = mergesort_avgrating(ptr);
        lp = merge_Audio(head, ptr);
    }
    return lp;
}

void Most_popular_Audiobook()
{
    Audio *ptr = Mainlist;
    /*while (ptr != NULL)
    {
        set_AverageRating(ptr);
        printf("%f\n", ptr->rating);
        ptr = ptr->audionext;
    }*/
    Mainlist = mergesort_avgrating(Mainlist);
    if (Mainlist->rating != 0)
    {
        printf("Most popular Audiobook Id:%d\n", Mainlist->AudiobookID);
        printf("Title:%s\n", Mainlist->Title);
        printf("Author:%s\n", Mainlist->Author);
        printf("Average Rating:%f\n", Mainlist->rating);
    }
    else
    {
        printf("No ratings are available to determine popularity");
    }
    Mainlist = mergesort_AudioID(Mainlist);
}

void Listening_history_report_user()
{
    int uid;
    Timestamp total;
    total.h = total.m = total.s = 0;
    printf("Enter User ID:");
    scanf("%d", &uid);
    user *uptr = return_userptr(uid);

    userAudio *aptr = uptr->audiolist;
    while (aptr != NULL)
    {
        if (aptr->timestamp.h != 0 || aptr->timestamp.m != 0 || aptr->timestamp.s != 0)
        {
            Audio *nptr = return_audioptr_of_Main(aptr->userA_ID);
            printf("Audiobook ID:%d\n", nptr->AudiobookID);
            printf("Title:%s\n", nptr->Title);
            printf("Author:%s\n", nptr->Author);
            printf("Timestamp:%d:%d:%d\n", aptr->timestamp.h, aptr->timestamp.m, aptr->timestamp.s);
            total.s += aptr->timestamp.s;
            total.m += aptr->timestamp.m;
            total.h += aptr->timestamp.h;
        }
        aptr = aptr->userA_next;
    }
    if (total.s >= 60)
    {
        total.m += (total.s / 60);
        total.s = (total.s % 60);
    }
    if (total.m >= 60)
    {
        total.h += (total.m / 60);
        total.m = (total.m % 60);
    }
    printf("Total listening time of user is:%d:%d:%d", total.h, total.m, total.s);
}

void Listening_history_report_audiobook()
{
    int aid;
    Timestamp total;
    total.h = total.m = total.s = 0;
    printf("Enter audiobook ID:");
    scanf("%d", &aid);
    Audio *ptr = return_audioptr_of_Main(aid);
    user *uptr = Userlist;
    while (uptr != NULL)
    {
        int flag = 0;
        userAudio *aptr = uptr->audiolist;
        while (aptr != NULL && flag == 0)
        {
            if (aptr->userA_ID == aid)
            {
                total.h += aptr->timestamp.h;
                total.m += aptr->timestamp.m;
                total.s += aptr->timestamp.s;
                flag = 1;
            }
            aptr = aptr->userA_next;
        }
        uptr = uptr->usernext;
    }
    if (total.s >= 60)
    {
        total.m += (total.s / 60);
        total.s = (total.s % 60);
    }
    if (total.m >= 60)
    {
        total.h += (total.m / 60);
        total.m = (total.m % 60);
    }
    printf("Audiobook ID:%d\n", ptr->AudiobookID);
    printf("Title:%s\n", ptr->Title);
    printf("Author:%s\n", ptr->Author);
    printf("Total listening times of Audiobook is:%d:%d:%d", total.h, total.m, total.s);
}

void deleteLinkedList()
{
    Audio *current = Mainlist;
    Audio *nextNode = NULL;

    while (current != NULL)
    {
        nextNode = current->audionext;
        free(current);
        current = nextNode;
    }
    Mainlist = NULL;
}

int compareTitle(userAudio *ptr1, userAudio *ptr2)
{
    int ret_val = 0;
    Audio *aptr1 = return_audioptr_of_Main(ptr1->userA_ID);
    Audio *aptr2 = return_audioptr_of_Main(ptr2->userA_ID);

    if (strcmp(aptr1->Title, aptr2->Title) < 0)
    {
        ret_val = 1;
    }
    else
    {
        if (strcmp(aptr1->Title, aptr2->Title) == 0)
        {
            if (strcmp(aptr1->Author, aptr2->Author) < 0)
            {
                ret_val = 1;
            }
            else if (strcmp(aptr1->Author, aptr2->Author) == 0)
            {
                ret_val = -1;
            }
        }
    }
    return ret_val;
}
userAudio *merge_ulib(userAudio *lptr1, userAudio *lptr2)
{
    userAudio *ptr1, *ptr2, *result, *tail;
    if (lptr2 == NULL)
    {
        return lptr1;
    }
    else if (lptr1 == NULL)
    {
        return lptr2;
    }
    else
    {
        ptr1 = lptr1;
        ptr2 = lptr2;
        if (compareTitle(ptr1, ptr2) == 1)
        {
            result = lptr1;
            ptr1 = ptr1->userA_next;
        }
        else if (compareTitle(ptr1, ptr2) == -1)
        {
            result = lptr1;
            ptr1 = ptr1->userA_next;
            ptr2 = ptr2->userA_next;
        }
        else
        {
            result = lptr2;
            ptr2 = ptr2->userA_next;
        }
        tail = result;
        while (ptr1 != NULL && ptr2 != NULL)
        {
            if (compareTitle(ptr1, ptr2) == 1)
            {
                tail->userA_next = ptr1;
                tail = tail->userA_next;
                ptr1 = ptr1->userA_next;
            }
            else if (compareTitle(ptr1, ptr2) == -1)
            {
                tail->userA_next = ptr1;
                tail = tail->userA_next;
                ptr1 = ptr1->userA_next;
                ptr2 = ptr2->userA_next;
            }
            else
            {
                tail->userA_next = ptr2;
                tail = tail->userA_next;
                ptr2 = ptr2->userA_next;
            }
        }
        if (ptr1 != NULL)
        {
            tail->userA_next = ptr1;
        }
        if (ptr2 != NULL)
        {
            tail->userA_next = ptr2;
        }
    }
    return result;
}

userAudio *mergesort_ulib(userAudio *lptr)
{
    userAudio *ptr;
    userAudio *head = lptr;
    if ((head != NULL) && (head->userA_next != NULL))
    {
        ptr = Divide(lptr);
        head = mergesort_ulib(head);
        ptr = mergesort_ulib(ptr);
        lptr = merge_ulib(head, ptr);
    }
    return lptr;
}

void MergeUserLibraries()
{
    int uid1, uid2;
    printf("Enter the id of 1st user:");
    scanf("%d", &uid1);
    printf("\nEnter the id of 2nd user:");
    scanf("%d", &uid2);
    user *u1 = return_userptr(uid1);
    user *u2 = return_userptr(uid2);
    u1->audiolist = mergesort_ulib(u1->audiolist);
    u2->audiolist = mergesort_ulib(u2->audiolist);
    u1->audiolist = merge_ulib(u1->audiolist, u2->audiolist);
    userAudio *head, *temp = NULL;
    userAudio *ptr = u1->audiolist;
    while (ptr != NULL)
    {
        userAudio *nptr = (userAudio *)malloc(sizeof(userAudio));
        nptr->userA_ID = ptr->userA_ID;
        nptr->timestamp.h = ptr->timestamp.h;
        nptr->timestamp.m = ptr->timestamp.m;
        nptr->timestamp.s = ptr->timestamp.s;
        nptr->rating = ptr->rating;
        nptr->userA_next = NULL;
        if (temp == NULL)
        {
            temp = nptr;
            head = temp;
        }
        else
        {
            temp->userA_next = nptr;
            temp = nptr;
        }
        ptr = ptr->userA_next;
    }
    u2->audiolist = head;
    display(Userlist);
    u1->audiolist = mergesort_UaudioID(u1->audiolist);
    u2->audiolist = mergesort_UaudioID(u2->audiolist);
}
int main()
{
    CreateMainList();
    CreateUserList();
    display(Userlist);
    int choice;
    printf("You have following choices:\n");
    printf("1.Add Audiobook\n2.Edit Audiobook\n3.Delete Audiobook\n4.Search Audiobook\n5.Add user profile\n6.Add to user library\n7.Listening Progress\n8.Display user library\n9.Rate Audiobbook\n10.Most popular audiobook\n11.Listening history report user\n12.Merge User libraries\n13.Listening history report audiobook\n14.Display list sorted according to average rating\n15.Exit\n");

    while (1)
    {
        printf("Enter your choice\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            Add_Audiobook();
            break;
        case 2:
            Edit_Audiobook();
            break;
        case 3:
            Delete_Audiobook();
            break;
        case 4:
            Search_Audiobook();
            break;
        case 5:
            Add_user_profile(Userlist);
            break;
        case 6:
            Add_to_user_library(Userlist);
            break;
        case 7:
            ListeningProgress();
            break;
        case 8:
            Display_User_Library();
            break;
        case 9:
            Rate_Audiobook();
            break;
        case 10:
            Most_popular_Audiobook();
            break;
        case 11:
            Listening_history_report_user();
            break;
        case 12:
            MergeUserLibraries();
            break;
        case 13:
            Listening_history_report_audiobook();
            break;
        case 14:
        {
            Mainlist = mergesort_avgrating(Mainlist);
            displaylist();
            Mainlist = mergesort_AudioID(Mainlist);
            break;
        }
        case 15:
            printf("Exiting the program\n");
			exit(0);
        default:
            break;
        }
    }
    delete_user_list(Userlist);
    deleteLinkedList();
}