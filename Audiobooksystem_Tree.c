#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 100

typedef enum {True,False} Boolean;

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
    float sum_rating;
    int count_rating;
    struct audio_tag *left;
    struct audio_tag *right;
    int level;
} Audio;

typedef struct userAudio_tag                //structure defined for audiolist of a particular user
{
    int userA_ID;
    int hours;
    int mins;
    int sec;
    float rating;
    struct userAudio_tag *userA_left;
    struct userAudio_tag *userA_right;
    int level;
} userAudio;

typedef struct user_tag                    //structure defined for users
{
    int UserID;
    char Name[MAX_SIZE];
    char Email[MAX_SIZE];
    char preference[MAX_SIZE];
    struct user_tag *user_left;
    struct user_tag *user_right;
    int level;
    userAudio *audiolist;
} user;

Audio *Mainlist;
user* Userlist;

Audio *CreateNode(int ID, char title[], char author[], char narrator[], int hrs, int min, int s, char genre[], float rating, int count)
{
    Audio *nptr = (Audio *)malloc(sizeof(Audio));
    nptr->AudiobookID = ID;
    strcpy(nptr->Title, title);
    strcpy(nptr->Author, author);
    strcpy(nptr->Narrator, narrator);
    nptr->hours = hrs;
    nptr->mins = min;
    nptr->sec = s;
    strcpy(nptr->genre, genre);
    nptr->sum_rating = rating;
    nptr->count_rating = count;
    nptr->left = NULL;
    nptr->right = NULL;
    nptr->level = 1;

    return nptr;
}

int level(Audio *temp)
{
    if (temp == NULL)
        return 0;
    return temp->level;
}

int Max(int l1, int l2)
{
    if (l1 > l2)
        return l1;
    else
        return l2;
}

int balancefactor(Audio *nptr)
{
    if (nptr == NULL)
        return 0;
    return (level(nptr->left) - level(nptr->right));
}

Audio *rotateright(Audio *r)
{
    Audio *tnode = r;
    if ((r == NULL) || (r->left == NULL))
    {
        printf("Error");
    }
    else
    {
        tnode = r->left;
        r->left = tnode->right;
        tnode->right = r;
        r->level = 1 + Max(level(r->left), level(r->right));
        tnode->level = 1 + Max(level(tnode->left), level(tnode->right));
    }
    return tnode;
}

Audio *rotateleft(Audio *r)
{
    Audio *tnode = r;
    if ((r == NULL) || (r->right == NULL))
    {
        printf("Error");
    }
    else
    {
        tnode = r->right;
        r->right = tnode->left;
        tnode->left = r;
        r->level = 1 + Max(level(r->left), level(r->right));
        tnode->level = 1 + Max(level(tnode->left), level(tnode->right));
    }
    return tnode;
}

Audio *insert_audiobook(Audio *root, int ID, char title[], char author[], char narrator[], int hrs, int min, int s, char genre[], float sum_rating, int count)
{
    if (root == NULL)
    {
        root = CreateNode(ID, title, author, narrator, hrs, min, s, genre, sum_rating, count);
    }
    else
    {
        if (ID < root->AudiobookID)
        {
            root->left = insert_audiobook(root->left, ID, title, author, narrator, hrs, min, s, genre, sum_rating, count);
        }
        else if (ID > root->AudiobookID)
        {
            root->right = insert_audiobook(root->right, ID, title, author, narrator, hrs, min, s, genre, sum_rating, count);
        }

        root->level = 1 + Max(level(root->left), level(root->right));

        int bfactor = balancefactor(root);

        if (bfactor > 1)
        {
            if (ID < root->left->AudiobookID) // LL
            {
                root = rotateright(root);
            }
            else if (ID > root->left->AudiobookID) // LR case
            {
                root->left = rotateleft(root->left);
                root = rotateright(root);
            }
        }
        else if (bfactor < -1)
        {
            if (ID > root->right->AudiobookID) // RR
            {
                root = rotateleft(root);
            }
            else if (ID < root->right->AudiobookID) // RL case
            {
                root->right = rotateright(root->right);
                root = rotateleft(root);
            }
        }
    }
    return root;
}

void CreateMainList()
{
    Mainlist = NULL;
    FILE *fptr = fopen("audio.csv", "r");
    if (fptr == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    char line[MAX_SIZE];
    int AudiobookID;
    char Title[MAX_SIZE], author[MAX_SIZE], narrator[MAX_SIZE], genre[MAX_SIZE];
    int hrs, mins, s;
    float sum_rating;
    int count;
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%d:%d:%d,%[^,],%f,%d", &AudiobookID, Title, author, narrator, &hrs, &mins, &s, genre, &sum_rating, &count);
        Mainlist = insert_audiobook(Mainlist, AudiobookID, Title, author, narrator, hrs, mins, s, genre, sum_rating, count);
    }
    fclose(fptr);
}

void inorder_display(Audio *ptr)
{
    if (ptr != NULL)
    {
        inorder_display(ptr->left);
        printf("id:%d\n", ptr->AudiobookID);
        printf("title:%s\n", ptr->Title);
        printf("author:%s\n", ptr->Author);
        printf("narrator:%s\n", ptr->Narrator);
        printf("duration %d:%d:%d\n", ptr->hours, ptr->mins, ptr->sec);
        printf("genre:%s\n", ptr->genre);
        printf("sum_rating:%f\n", ptr->sum_rating);
        printf("count_rating:%d\n", ptr->count_rating);
        inorder_display(ptr->right);
    }
}

Audio *return_audioptr_of_main(Audio *root, int id)
{
    Audio *ptr;
    if (root == NULL || root->AudiobookID == id)
    {
        ptr = root;
    }
    else if (root->AudiobookID > id)
    {
        ptr = return_audioptr_of_main(root->left, id);
    }
    else if (root->AudiobookID < id)
    {
        ptr = return_audioptr_of_main(root->right, id);
    }
    return ptr;
}

void Add_Audiobook()
{
    printf("Enter details of the audiobook\n");
    int id, hrs, mins, s;
    char title[MAX_SIZE], author[MAX_SIZE], narrator[MAX_SIZE], genre[MAX_SIZE];
    float sum_rating = 0.0;
    int count = 0;

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

    Mainlist = insert_audiobook(Mainlist, id, title, author, narrator, hrs, mins, s, genre, sum_rating, count);

    printf("\nThe audiobook has been successfully added");
}

void Edit_audiobook()
{
    int id;
    printf("enter the audiobook id that you want to edit:");
    scanf("%d", &id);

    Audio *ptr = return_audioptr_of_main(Mainlist, id);
    if (ptr != NULL)
    {
        printf("enter the field you want to edit:\n1.Title\n2.Author\n3.Narrator\n4.Duration\n5.genre\nEnter choice:");
        int choice;
        scanf("%d", &choice);
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
    }
    else
    {
        printf("\n No such Audiobook Exists");
    }
}

void SearchRecursive(Audio *root, char str[])
{
    if (root != NULL)
    {
        SearchRecursive(root->left, str);
        if ((strcmp(root->Title, str) == 0) || (strcmp(root->Author, str) == 0) || (strcmp(root->genre, str) == 0)) // traversal to search the audiobook
        {
            printf("id=%d\n", root->AudiobookID);
            printf("title=%s\n", root->Title);
            printf("\n");
        }
        SearchRecursive(root->right, str);
    }
}

void Search_Audiobook()
{
    //Boolean val;
    printf("Enter title,author or genre which you want to search:");
    char str[MAX_SIZE];
    scanf(" %[^\n]", str);

    SearchRecursive(Mainlist,str);
}

Audio* setavl(Audio *root,Audio *ptr)
{
	if (root != NULL)
	{
		if (root->AudiobookID > ptr->AudiobookID)
		{
			root->left = setavl(root->left, ptr);
		}
		else if (root->AudiobookID < ptr->AudiobookID)
		{
			root->right = setavl(root->right, ptr);
		}

		root->level = 1 + Max(level(root->left), level(root->right));

		int bal = balancefactor(root);
        
         //RL case
		if (bal < -1 && ptr->AudiobookID < root->right->AudiobookID)
		{
			root->right = rotateright(root->right);
			root = rotateleft(root);
		}
		// RR case
		else if (bal < -1 && ptr->AudiobookID > root->right->AudiobookID)
		{
			root = rotateleft(root);
		}
		// LL case
		else if (bal > 1 && ptr->AudiobookID < root->left->AudiobookID)
		{
			root = rotateright(root);
		}
		// LR case
		else if (bal > 1 && ptr->AudiobookID > root->left->AudiobookID)
		{
			root->left = rotateleft(root->left);
			root = rotateright(root);
		}

	}
	return root;
}

Audio* DeleteNode(Audio* root,int k)
{
	Audio* nptr;
	if(root==NULL)
	{
		printf("Error");
	}

	if(k<root->AudiobookID)                             //we need to move left if key is less than root's value
	{
		root->left=DeleteNode(root->left,k);
	}
	else if(k>root->AudiobookID)                          //we need to move right if key is greater than root's value
	{
		root->right=DeleteNode(root->right,k);
	}
	else               //if key to be deleted is found
	{
		if(root->left==NULL || root->right==NULL)                     //single or zero child
		{
			Audio* temp;
			if(root->left!=NULL) temp=root->left;
			else temp=root->right;

			if(temp==NULL)            //zero child
			{
				temp=root;
				root=NULL;
				free(temp);
			}
			else 
			{
				Audio *ptr=root;
				root = temp;
				free(ptr);
			}      //single child

		}
		else // both child present
		{
			Audio *prev, *ptr;
			Audio *nptr=root;
			
			prev = ptr = root->right;
			while (ptr->left != NULL)
			{
				prev = ptr;
				ptr = ptr->left;
			}
			if (prev == ptr)
			{
				ptr->left = nptr->left;
				root = ptr;
				free(nptr);
			}
			else
			{
				prev->left = ptr->right;
				ptr->right = nptr->right;
				ptr->left = nptr->left;
				root = ptr;
				free(nptr);
			}
            root = setavl(root,prev);
		}
		
	}
	if(root!=NULL)
	{
		root->level=1+Max(level(root->left),level(root->right));
		int balance=balancefactor(root);

		//LL case
		if(balance>1 && balancefactor(root->left)>=0)
		{
			root=rotateright(root);
		}
		else if(balance>1 && balancefactor(root->left)<0)                     //LR case
		{
			root->left=rotateleft(root->left);
			root=rotateright(root);
		}
		else if(balance<-1 && balancefactor(root->right)<=0)              //RR case
		{
			root=rotateleft(root);
		}
		else if(balance<-1 && balancefactor(root->right)>0)                 //RL case
		{
			root->right=rotateright(root->right);
			root=rotateleft(root);
		}
	}
	return root;
}

userAudio* setavl_user(userAudio *,userAudio *);
userAudio* DeleteNode_userA(userAudio* ,int );
void inorder_user(user *);

user* Delete_from_Userlist(user* root,int k)
{
    if(root!=NULL)
    {
        root->user_left = Delete_from_Userlist(root->user_left,k);
        if (root->audiolist != NULL)
        {
            userAudio *updatedAudiolist = DeleteNode_userA(root->audiolist, k);
            if (updatedAudiolist != NULL)
            {
                root->audiolist = updatedAudiolist;
            }
        }
        root->user_right = Delete_from_Userlist(root->user_right,k);
    }
    return root;
}

void SearchRecursive_Delete(Audio *root, char str[])
{
    if (root != NULL)
    {
        SearchRecursive_Delete(root->left, str);
        if ((strcmp(root->Title, str) == 0) || (strcmp(root->Author, str) == 0) || (strcmp(root->genre, str) == 0)) // traversal to search the audiobook
        {
            int k = root->AudiobookID;
            Mainlist=DeleteNode(Mainlist,root->AudiobookID);
            Userlist = Delete_from_Userlist(Userlist,k);
        }
        SearchRecursive_Delete(root->right, str);
    }
}

void Delete_Audiobook()
{
    printf("enter the Audiobook Title or Author you want to delete:");
    char str[MAX_SIZE];
    scanf(" %[^\n]", str);
    SearchRecursive_Delete(Mainlist,str);
    inorder_user(Userlist);
    inorder_display(Mainlist);
}

int userlevel(user *temp)
{
    if (temp == NULL)
        return 0;
    return temp->level;
}

int user_balancefactor(user *nptr)
{
    if (nptr == NULL)
        return 0;
    return (userlevel(nptr->user_left) - userlevel(nptr->user_right));
}

user *user_rotateright(user *r)
{
    user *tnode = r;
    if ((r == NULL) || (r->user_left == NULL))
    {
        printf("Error");
    }
    else
    {
        tnode = r->user_left;
        r->user_left = tnode->user_right;
        tnode->user_right = r;
        r->level = 1 + Max(userlevel(r->user_left), userlevel(r->user_right));
        tnode->level = 1 + Max(userlevel(tnode->user_left), userlevel(tnode->user_right));
    }
    return tnode;
}

user *user_rotateleft(user *r)
{
    user *tnode = r;
    if ((r == NULL) || (r->user_right == NULL))
    {
        printf("Error");
    }
    else
    {
        tnode = r->user_right;
        r->user_right = tnode->user_left;
        tnode->user_left = r;
        r->level = 1 + Max(userlevel(r->user_left), userlevel(r->user_right));
        tnode->level = 1 + Max(userlevel(tnode->user_left), userlevel(tnode->user_right));
    }
    return tnode;
}

int userA_level(userAudio *temp)
{
    if (temp == NULL)
        return 0;
    return temp->level;
}

int userA_balancefactor(userAudio *nptr)
{
    if (nptr == NULL)
        return 0;
    return (userA_level(nptr->userA_left) - userA_level(nptr->userA_right));
}

userAudio *userA_rotateright(userAudio *r)
{
    userAudio *tnode = r;
    if ((r == NULL) || (r->userA_left == NULL))
    {
        printf("Error");
    }
    else
    {
        tnode = r->userA_left;
        r->userA_left = tnode->userA_right;
        tnode->userA_right = r;
        r->level = 1 + Max(userA_level(r->userA_left), userA_level(r->userA_right));
        tnode->level = 1 + Max(userA_level(tnode->userA_left), userA_level(tnode->userA_right));
    }
    return tnode;
}

userAudio *userA_rotateleft(userAudio *r)
{
    userAudio *tnode = r;
    if ((r == NULL) || (r->userA_right == NULL))
    {
        printf("Error");
    }
    else
    {
        tnode = r->userA_right;
        r->userA_right = tnode->userA_left;
        tnode->userA_left = r;
        r->level = 1 + Max(userA_level(r->userA_left), userA_level(r->userA_right));
        tnode->level = 1 + Max(userA_level(tnode->userA_left), userA_level(tnode->userA_right));
    }
    return tnode;
}

user *insert_user(user *root,user *nptr)
{
    if (root == NULL)
    {
        root = nptr;
    }
    else
    {
        if (nptr->UserID < root->UserID)
        {
            root->user_left = insert_user(root->user_left, nptr);
        }
        else if (nptr->UserID > root->UserID)
        {
            root->user_right = insert_user(root->user_right,nptr);
        }

        root->level = 1 + Max(userlevel(root->user_left), userlevel(root->user_right));

        int bfactor = user_balancefactor(root);

        if (bfactor > 1)
        {
            if (nptr->UserID < root->user_left->UserID) // LL
            {
                root = user_rotateright(root);
            }
            else if (nptr->UserID > root->user_left->UserID) // LR case
            {
                root->user_left = user_rotateleft(root->user_left);
                root = user_rotateright(root);
            }
        }
        else if (bfactor < -1)
        {
            if (nptr->UserID > root->user_right->UserID) // RR
            {
                root = user_rotateleft(root);
            }
            else if (nptr->UserID < root->user_right->UserID) // RL case
            {
                root->user_right = user_rotateright(root->user_right);
                root = user_rotateleft(root);
            }
        }
    }
    return root;
}

userAudio *insert_useraudio(userAudio *root,userAudio *nptr)
{
    if (root == NULL)
    {
        root = nptr;
    }
    else
    {
        if (nptr->userA_ID < root->userA_ID)
        {
            root->userA_left = insert_useraudio(root->userA_left, nptr);
        }
        else if (nptr->userA_ID > root->userA_ID)
        {
            root->userA_right = insert_useraudio(root->userA_right,nptr);
        }

        root->level = 1 + Max(userA_level(root->userA_left), userA_level(root->userA_right));

        int bfactor = userA_balancefactor(root);

        if (bfactor > 1)
        {
            if (nptr->userA_ID < root->userA_left->userA_ID) // LL
            {
                root = userA_rotateright(root);
            }
            else if (nptr->userA_ID > root->userA_left->userA_ID) // LR case
            {
                root->userA_left = userA_rotateleft(root->userA_left);
                root = userA_rotateright(root);
            }
        }
        else if (bfactor < -1)
        {
            if (nptr->userA_ID > root->userA_right->userA_ID) // RR
            {
                root = userA_rotateleft(root);
            }
            else if (nptr->userA_ID < root->userA_right->userA_ID) // RL case
            {
                root->userA_right = userA_rotateright(root->userA_right);
                root = userA_rotateleft(root);
            }
        }
    }
    return root;
}

void CreateUserlist()
{
    FILE *fptr = fopen("Userlist.txt", "r");
    char line[MAX_SIZE];
    Userlist = NULL;
    user *usertemp;

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
                usertemp->audiolist = NULL;
                usertemp->user_left = NULL;
                usertemp->user_right = NULL;
                usertemp->level = 1;

                case_no = 1;
            }
            else if (strncmp(line, "#", 1) == 0)
            {
                Userlist = insert_user(Userlist,usertemp);
            }
            else
            {
                if (strncmp(line, "Audio", 5) == 0)
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
                        case_no++;
                        break;
                    }
                    case 4:
                    {
                        strcpy(usertemp->preference, line);
                        break;
                    }
                    case 5:
                    {
                        userAudio *nptr = (userAudio *)malloc(sizeof(userAudio));
                        nptr->userA_ID = atoi(line);
                        nptr->rating = 0;
                        nptr->hours = 0;
                        nptr->mins = 0;
                        nptr->sec = 0;
                        nptr->level = 1;
                        nptr->userA_left = NULL;
                        nptr->userA_right = NULL;
                        usertemp->audiolist = insert_useraudio(usertemp->audiolist,nptr);
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
} 

void Add_User_profile()
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
    printf("\nEnter your preference:");
    scanf(" %[^\n]", preferences);
    newuser->audiolist = NULL;
    newuser->user_left=NULL;
    newuser->user_right=NULL;
    newuser->level=1;

    Userlist=insert_user(Userlist,newuser);
}

void inorder_Uaudio(userAudio *root)
{
    if(root!=NULL)
    {
        inorder_Uaudio(root->userA_left);
        printf("\n%d", root->userA_ID);
        printf("\n%d:%d:%d", root->hours, root->mins, root->sec);
        printf("\n%f", root->rating);
        inorder_Uaudio(root->userA_right);
    }
}

user *return_userptr(user *root, int id)
{
    user *ptr;
    if (root == NULL || root->UserID == id)
    {
        ptr = root;
    }
    else if (root->UserID > id)
    {
        ptr = return_userptr(root->user_left, id);
    }
    else if (root->UserID < id)
    {
        ptr = return_userptr(root->user_right, id);
    }
    return ptr;
}

userAudio *return_audioptr_of_user(userAudio *root, int id)
{
    userAudio *ptr;
    if (root == NULL || root->userA_ID == id)
    {
        ptr = root;
    }
    else if (root->userA_ID > id)
    {
        ptr = return_audioptr_of_user(root->userA_left, id);
    }
    else if (root->userA_ID < id)
    {
        ptr = return_audioptr_of_user(root->userA_right, id);
    }
    return ptr;
}

void Add_to_UserLibrary()
{
    int uid, n, aid;
    printf("Enter your User ID:");
    scanf("%d", &uid);
    printf("Enter audioBook Id which you want to add");
    scanf("%d", &aid);

    user* uptr=return_userptr(Userlist,uid);
    userAudio* nptr=(userAudio*)malloc(sizeof(userAudio));
    nptr->userA_ID=aid;
    nptr->userA_left=NULL;
    nptr->userA_right=NULL;
    nptr->level=1;
    nptr->hours=0;
    nptr->mins=0;
    nptr->sec=0;
    nptr->rating=0.0;

    uptr->audiolist=insert_useraudio(uptr->audiolist,nptr);
}

userAudio* setavl_user(userAudio *root,userAudio *ptr)
{
	if (root != NULL)
	{
		if (root->userA_ID > ptr->userA_ID)
		{
			root->userA_left = setavl_user(root->userA_left, ptr);
		}
		else if (root->userA_ID < ptr->userA_ID)
		{
			root->userA_right = setavl_user(root->userA_right, ptr);
		}

		root->level = 1 + Max(userA_level(root->userA_left), userA_level(root->userA_right));

		int bal = userA_balancefactor(root);

		// RL case
		if (bal < -1 && ptr->userA_ID < root->userA_right->userA_ID)
		{
			root->userA_right = userA_rotateright(root->userA_right);
			root = userA_rotateleft(root);
		}
		// RR case
		else if (bal < -1 && ptr->userA_ID > root->userA_right->userA_ID)
		{
			root = userA_rotateleft(root);
		}
		// LL case
		else if (bal > 1 && ptr->userA_ID < root->userA_left->userA_ID)
		{
			root = userA_rotateright(root);
		}
		// LR case
		else if (bal > 1 && ptr->userA_ID > root->userA_left->userA_ID)
		{
			root->userA_left = userA_rotateleft(root->userA_left);
			root = userA_rotateright(root);
		}
    }
	return root;
}

userAudio* DeleteNode_userA(userAudio* root,int k)
{
    if (root != NULL)
    {
        if (k < root->userA_ID) // we need to move left if key is less than root's value
        {
            root->userA_left = DeleteNode_userA(root->userA_left, k);
        }
        else if (k > root->userA_ID) // we need to move right if key is greater than root's value
        {
            root->userA_right = DeleteNode_userA(root->userA_right, k);
        }
        else // if key to be deleted is found
        {
            if (root->userA_left == NULL || root->userA_right == NULL) // single or zero child
            {
                userAudio *temp;
                if (root->userA_left != NULL)
                    temp = root->userA_left;
                else
                    temp = root->userA_right;

                if (temp == NULL) // zero child
                {
                    temp = root;
                    root = NULL;
                    free(temp);
                }
                else
                {
                    userAudio *ptr = root;
                    root = temp;
                    free(ptr);
                } // single child
            }
            else // both child present
            {
                userAudio *prev, *ptr;
                userAudio *nptr = root;

                prev = ptr = root->userA_right;
                while (ptr->userA_left != NULL)
                {
                    prev = ptr;
                    ptr = ptr->userA_left;
                }
                if (prev == ptr)
                {
                    ptr->userA_left = nptr->userA_left;
                    root = ptr;
                    free(nptr);
                }
                else
                {
                    prev->userA_left = ptr->userA_right;
                    ptr->userA_right = nptr->userA_right;
                    ptr->userA_left = nptr->userA_left;
                    root = ptr;
                    free(nptr);
                }
                root = setavl_user(root, prev);
            }
        }
        if (root != NULL)
        {
            root->level = 1 + Max(userA_level(root->userA_left), userA_level(root->userA_right));
            int balance = userA_balancefactor(root);

            // LL case
            if (balance > 1 && userA_balancefactor(root->userA_left) >= 0)
            {
                root = userA_rotateright(root);
            }
            else if (balance > 1 && userA_balancefactor(root->userA_left) < 0) // LR case
            {
                root->userA_left = userA_rotateleft(root->userA_left);
                root = userA_rotateright(root);
            }
            else if (balance < -1 && userA_balancefactor(root->userA_right) <= 0) // RR case
            {
                root = userA_rotateleft(root);
            }
            else if (balance < -1 && userA_balancefactor(root->userA_right) > 0) // RL case
            {
                root->userA_right = userA_rotateright(root->userA_right);
                root = userA_rotateleft(root);
            }
        }
    }
    return root;
}

void Remove_from_Userlibrary()
{
    int uid, n, aid;
    printf("Enter your User ID:");
    scanf("%d", &uid);
    printf("Enter audioBook Id which you want to remove:");
    scanf("%d", &aid);

    user* ptr=return_userptr(Userlist,uid);
    ptr->audiolist=DeleteNode_userA(ptr->audiolist,aid);
    inorder_user(Userlist);
}

void inorder_user(user *root)
{
    if(root!=NULL)
    {
        inorder_user(root->user_left);
        printf("%d\n", root->UserID);
        printf("%s\n", root->Name);
        printf("%s\n", root->Email);
        printf("%s\n",root->preference);
        inorder_Uaudio(root->audiolist);
        inorder_user(root->user_right);
    }
}

void Listening_Progress()
{
    int uid, aid, h, m, s;
    printf("Enter User ID");
    scanf("%d", &uid);
    printf("\nEnter Audiobook ID");
    scanf("%d", &aid);
    char time[10];
    printf("\nEnter listening progress in hh:mm:ss format");
    scanf("%s", time);

    user *ptr = return_userptr(Userlist,uid);

    userAudio* Aptr=return_audioptr_of_user(ptr->audiolist,aid);

    sscanf(time, "%d:%d:%d", &h, &m, &s);
    Aptr->hours=h;
    Aptr->mins=m;
    Aptr->sec=s;
 
    if (Aptr->sec>= 60)                //care has been taken of the fact if the seconds' part in timestamp becomes>60
    {
        Aptr->mins += (Aptr->sec / 60);
        Aptr->sec = (Aptr->sec % 60);
    }
    if (Aptr->mins >= 60)
    {
        Aptr->hours += (Aptr->mins / 60);
        Aptr->mins = (Aptr->mins % 60);
    }
    printf("timestamp has been successfully updated to %d:%d:%d", Aptr->hours, Aptr->mins, Aptr->sec);
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

    user *temp = return_userptr(Userlist,uid);
    userAudio *ptr = return_audioptr_of_user(temp->audiolist,aid);
    ptr->rating = rate;

    Audio *aptr = return_audioptr_of_main(Mainlist,aid);
    aptr->sum_rating += rate;
    aptr->count_rating += 1;
}

void totaltime_user(userAudio *root,int *hr,int *min,int *sec)
{
    if(root!=NULL)
    {
        totaltime_user(root->userA_left,hr,min,sec);
        if (root->hours != 0 || root->mins != 0 || root->sec != 0)
        {
            Audio *nptr = return_audioptr_of_main(Mainlist,root->userA_ID);
            printf("Audiobook ID:%d\n", nptr->AudiobookID);
            printf("Title:%s\n", nptr->Title);
            printf("Author:%s\n", nptr->Author);
            printf("Timestamp:%d:%d:%d\n", root->hours,root->mins,root->sec);
            *hr = *hr + root->hours;
            *min = *min + root->mins;
            *sec = *sec + root->sec;
        }
        totaltime_user(root->userA_right,hr,min,sec);
    }
}

void Listening_history_report_user()
{
    int uid;
    int t_hrs,t_min,t_sec;
    t_hrs=t_min=t_sec=0;
    printf("Enter User ID:");
    scanf("%d", &uid);
    user *ptr = return_userptr(Userlist,uid);
    totaltime_user(ptr->audiolist,&t_hrs,&t_min,&t_sec);
    if (t_sec >= 60)
    {
        t_min += (t_sec / 60);
        t_sec = (t_sec % 60);
    }
    if (t_min >= 60)
    {
        t_hrs += (t_min / 60);
        t_min = (t_min % 60);
    }
    printf("Total listening time of user is:%d:%d:%d", t_hrs,t_min,t_sec);
}

void totaltime_audio(user* root,int *hr,int *min,int *sec,int aid)
{
    if (root!=NULL)
    {
        totaltime_audio(root->user_left,hr,min,sec,aid);
        userAudio *ptr = return_audioptr_of_user(root->audiolist,aid);
        if(ptr!=NULL)
        {
            *hr += ptr->hours;
            *min += ptr->mins;
            *sec += ptr->sec;
        }
        totaltime_audio(root->user_right,hr,min,sec,aid);
    }
}

void Listening_history_report_audiobook()
{
    int aid;
    int t_hrs,t_min,t_sec;
    t_hrs=t_min=t_sec=0;
    printf("Enter audiobook ID:");
    scanf("%d", &aid);
    Audio *ptr = return_audioptr_of_main(Mainlist,aid);
    totaltime_audio(Userlist,&t_hrs,&t_min,&t_sec,aid);
    if (t_sec >= 60)
    {
        t_min += (t_sec / 60);
        t_sec = (t_sec % 60);
    }
    if (t_min >= 60)
    {
        t_hrs += (t_min / 60);
        t_min = (t_min % 60);
    }
    printf("Audiobook ID:%d\n", ptr->AudiobookID);
    printf("Title:%s\n", ptr->Title);
    printf("Author:%s\n", ptr->Author);
    printf("Total listening times of Audiobook is:%d:%d:%d",t_hrs,t_min,t_sec);
}

Audio* Max_avg_rating(Audio* root,float* max_avg)
{
    Audio* result=root;
    if(root!=NULL)
    {
        result=Max_avg_rating(root->left,max_avg);
        if(result!=NULL) printf("%dleft\n",result->AudiobookID);
        if(root->count_rating!=0)
        {
            float avg = (root->sum_rating / (float)root->count_rating);
            if (avg > *max_avg)
            {
                *max_avg = avg;
                result = root;
                //printf("%droot",result->AudiobookID);
            }
        }
        if(root->right!=NULL)
        {
            result=Max_avg_rating(root->right,max_avg);
            //printf("%dright",result->AudiobookID);
        }
    }
    return result;
}

void Most_Popular_Audiobook()
{
    float val=0.0;
    Audio* ptr=Max_avg_rating(Mainlist,&val);
   // printf("%d",ptr->AudiobookID);
    if (ptr!=NULL)
    {
        printf("Most popular Audiobook Id:%d\n", ptr->AudiobookID);
        printf("Title:%s\n", ptr->Title);
        printf("Author:%s\n", ptr->Author);
        float avg=ptr->sum_rating/ptr->count_rating;
        printf("Average Rating:%f\n",avg);
    }
    else
    {
        printf("No ratings are available to determine popularity");
    }
}

int compareTime(userAudio* audio1,userAudio* audio2)
{
    int ans=1;
    if(audio1->hours<audio2->hours)
    {
        ans=0;
    }
    else if(audio1->hours==audio2->hours)
    {
        if(audio1->mins<audio2->mins)
        {
            ans=0;
        }
        else if(audio1->mins==audio2->mins)
        {
            if(audio1->sec<audio2->sec)
            {
                ans=0;
            }
        }
    }
    return ans;
}

userAudio* Create_Newnode(userAudio* ptr)
{
    userAudio* nptr = (userAudio *)malloc(sizeof(userAudio));
    nptr->userA_ID = ptr->userA_ID;
    nptr->hours = ptr->hours;
    nptr->mins = ptr->mins;
    nptr->sec = ptr->sec;
    nptr->rating = ptr->rating;
    nptr->level = 1;
    nptr->userA_left = NULL;
    nptr->userA_right = NULL;
    return nptr;
}

userAudio* Insertion_Using_Time(userAudio* root,userAudio* nptr)
{
    if (root == NULL)
    {
        root = nptr;
    }
    else
    {
        if (compareTime(root,nptr)==1)
        {
            root->userA_left = Insertion_Using_Time(root->userA_left, nptr);
        }
        else if (compareTime(root,nptr)==0)
        {
            root->userA_right = Insertion_Using_Time(root->userA_right,nptr);
        }

        root->level = 1 + Max(userA_level(root->userA_left), userA_level(root->userA_right));

        int bfactor = userA_balancefactor(root);

        if (bfactor > 1)
        {
            if (compareTime(root->userA_left,nptr)==1) // LL
            {
                root = userA_rotateright(root);
            }
            else if (compareTime(root->userA_left,nptr)==0) // LR case
            {
                root->userA_left = userA_rotateleft(root->userA_left);
                root = userA_rotateright(root);
            }
        }
        else if (bfactor < -1)
        {
            if (compareTime(root->userA_right,nptr)==0) // RR
            {
                root = userA_rotateleft(root);
            }
            else if (compareTime(root->userA_right,nptr)==1) // RL case
            {
                root->userA_right = userA_rotateright(root->userA_right);
                root = userA_rotateleft(root);
            }
        }
    }
    return root;
}

userAudio* CreateNewTree(userAudio *root,userAudio *new)
{
    if (root != NULL)
    {
        new = CreateNewTree(root->userA_left, new);
        userAudio *ptr = Create_Newnode(root);
        new = Insertion_Using_Time(new, ptr);
        new = CreateNewTree(root->userA_right, new);
    }
    return new;
}

void PrintAudiobooks(userAudio* uptr,char str[])
{
    PrintAudiobooks(uptr->userA_left,str);
    Audio* aptr=return_audioptr_of_main(Mainlist,uptr->userA_ID);
    if((strcmp(str, aptr->Author) == 0) || (strcmp(str, aptr->Title) == 0) || (strcmp(str, aptr->Narrator) == 0) || (strcmp(str, aptr->genre) == 0))
    {
        printf("Audiobook ID:%d\n", uptr->userA_ID);
    }
    PrintAudiobooks(uptr->userA_right,str);
}

void PrintAllAudiobooks(userAudio* uptr)
{ 
    if(uptr!=NULL)
    {
        PrintAllAudiobooks(uptr->userA_left);
        printf("Audiobook ID:%d\n", uptr->userA_ID);
        PrintAllAudiobooks(uptr->userA_right);
    }
}

void deleteUserLib(userAudio *root) {
    if (root == NULL)
        return;

    deleteUserLib(root->userA_left);
    deleteUserLib(root->userA_right);
    free(root);
}

void Display_User_Libraries()
{
    int uid;
    char str[MAX_SIZE];
    printf("Enter The User ID:");
    scanf("%d", &uid);
    printf("Enter The filter criteria:");
    scanf(" %[^\n]", str);

    user* uptr=return_userptr(Userlist,uid);
    userAudio* ptr=NULL;
    ptr = CreateNewTree(uptr->audiolist,ptr);
   
    if (strcmp(str, "All Audiobooks") != 0)
    {
        PrintAudiobooks(ptr,str);
    }
    else
    {
        PrintAllAudiobooks(ptr);
    }
    deleteUserLib(ptr);
}

void RangeSearch(Audio* root,int low,int high)
{
	if(root!=NULL)
	{
		if(root->AudiobookID<low)
		{
			RangeSearch(root->right,low,high);
		}
		else if(root->AudiobookID>high)
		{
			RangeSearch(root->left,low,high);
		}
		else if(root->AudiobookID>=low && root->AudiobookID<=high)
		{
			RangeSearch(root->left,low,high);
			printf("%s\n",root->Title);
			RangeSearch(root->right,low,high);
		}
	}
}

void RangeSearch_Audio()
{
    printf("Enter range of Audiobook ID:");
    int max,min;
    scanf("%d%d",&max,&min);
    RangeSearch(Mainlist,max,min);
}

int main()
{
    CreateMainList();
    CreateUserlist();

    int choice;
    printf("You have following choices:\n");
    printf("1.Add Audiobook\n2.Edit Audiobook\n3.Delete Audiobook\n4.Search Audiobook\n5.Add user profile\n6.Add to user library\n7.Remove from user library\n8.Listening Progress\n9.Display user library\n10.Rate Audiobbook\n11.Most popular audiobook\n12.Listening history report user\n13.Listening history report audiobook\n14.Range search\n15.Exit\n");

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
            Edit_audiobook();
            break;
        case 3:
            Delete_Audiobook();
            break;
        case 4:
            Search_Audiobook();
            break;
        case 5:
            Add_User_profile();
            break;
        case 6:
            Add_to_UserLibrary();
            break;
        case 7:
            Remove_from_Userlibrary();
            break;
        case 8:
            Listening_Progress();
            break;
        case 9:
            Display_User_Libraries();
            break;
        case 10:
            Rate_Audiobook();
            break;
        case 11:
            Most_Popular_Audiobook();
            break;
        case 12:
            Listening_history_report_user();
            break;
        case 13:
            Listening_history_report_audiobook();
            break;
        case 14:
            RangeSearch_Audio();
            break;
        case 15:
            printf("Exiting the program\n");
			exit(0);
        default:
            break;
        }
    }
    return 0;
}