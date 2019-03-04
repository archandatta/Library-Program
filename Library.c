#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 32

typedef struct
{
    char title[MAX_STR];
    char author[MAX_STR];
    int yearPub;
} BookType;

typedef struct Node
{
    BookType *data;
    struct Node *next;
} NodeType;

typedef struct
{
    NodeType *head; // points to the fist node
    NodeType *tail; // point to the last node
} ListType;

//Prototypes
int initBook(BookType **book);
void initList(ListType *list);
void addByTitle(ListType *list, BookType *newBook);
void addByYear(ListType *list, BookType *newBook);
void copyList(ListType *src, ListType *dest);
void copyByYear(ListType *src, ListType *dest);
void delAuthor(ListType *list, char *name);
void printList(ListType *list);
void cleanupList(ListType *list);
void cleanupData(ListType *list);
int compareBooksByYear(BookType *, BookType *);
void addBook(ListType *, BookType *, int);
void printHead(ListType *list);
void printTail(ListType *list);

int main()
{
    //Initiaize variables
    BookType *book;
    char nameOfAuthor[MAX_STR];

    // Allocate memory to lists
    ListType *booksByTitle = (ListType *)malloc(sizeof(ListType));
    ListType *booksByYear = (ListType *)malloc(sizeof(ListType));
    ListType *tmpList = (ListType *)malloc(sizeof(ListType));

    // Initialize the lists
    initList(booksByTitle);
    initList(booksByYear);
    initList(tmpList);

    // Prompt user for input until they decide to end
    while (1)
    {
        int rc = initBook(&book);
        // check is the user wants to end or not
        if (rc == 0)
            break;
        // add the book to the list
        addByTitle(booksByTitle, book);
    }

    printf("*** BOOKS BY TITLE ***\n");
    printList(booksByTitle);

    // copy the list 
    copyList(booksByTitle, tmpList);

    printf("Enter a author would you like to delete?\n");
    fgets(nameOfAuthor, sizeof(nameOfAuthor), stdin);
    nameOfAuthor[strlen(nameOfAuthor) - 1] = '\0';

    // delete the inputed author
    delAuthor(tmpList, nameOfAuthor);

    printf("*** TEMP LIST ***\n");
    printList(tmpList);

    // copy the title list to the year list and print the yesr list 
    copyByYear(booksByTitle, booksByYear);
    printf("*** BOOKS BY TITLE ***\n");
    printList(booksByTitle);
    printf("*** BOOKS BY YEAR ***\n");
    printList(booksByYear);

    // clean up data of all the books
    cleanupData(booksByTitle);

    // clean up the list nodes
    cleanupList(booksByTitle);
    cleanupList(booksByYear);
    cleanupList(tmpList);

    // free the the lists
    free(booksByTitle);
    free(booksByYear);
    free(tmpList);

    return 0;
}

/*
  Function:  initBook
  Purpose:   Initialize the book by prompting to the user and obtaining the information 
  Parameters: 
	in:     double pointer of a booktype
	return: success or faliure value that is represented by a int value 
*/
int initBook(BookType **book)
{
    char title[MAX_STR];
    char author[MAX_STR];
    char yearS[MAX_STR];
    int year = 0;

    // prompt the user for the year of the book
    printf("What is the book title?\n");
    fgets(title, sizeof(title), stdin);
    title[strlen(title) - 1] = '\0';

    // checks if the user wants to end
    if (strcmp(title, "end") == 0)
    {
        // Data entry will end
        return 0;
    }
    else
    {
        

        // prompt the user for the author of the book
        printf("What is the book author?\n");
        fgets(author, sizeof(author), stdin);
        author[strlen(author) - 1] = '\0';

        // prompt the user for the year of the book
        printf("What is the year of publication?\n");
        fgets(yearS, sizeof(yearS), stdin);
        int rc = sscanf(yearS, "%d", &year);
        // if they enter a invlid year restart the input process
        if (rc == 0)
        {
            printf("Invalid integer, restarting input.. please try again\n");
            return 1;
        }
		// allocate the memory for each book
        *book = (BookType *)malloc(sizeof(BookType));
        //return success implying the new book is initialized
        strcpy((*book)->title, title);
        strcpy((*book)->author, author);
        (*book)->yearPub = year;
        printf("Success, your book is added\n");
        return 1;
    }
}

/*
  Function: initList
  Purpose:  to initialize a given list 
  Parameters: 
	in:     a pointer to a list
	return: -
*/
void initList(ListType *list)
{
    list->head = NULL;
    list->tail = NULL;
}
/*
  Function:  addByTitle 
  Purpose:   add a book by title to the given list in while maintaing acending order
  Parameters: 
	in:      list and book pointer 
	return:  the list with the added book in acending order
*/
void addByTitle(ListType *list, BookType *newBook)
{
    NodeType *currNode = list->head;
    int pos = 0;
    // figure out the position in the list where to add the book
    // when currNode is NULL, we will hit the end of the list
    while (currNode != NULL)
    {
        // Compare the given to the book at the current node to find at what position the book should be added
        int compareResult = (strcmp(newBook->title, currNode->data->title));

        // if the title are the same
        if (compareResult == 0 || compareResult < 0)
            break;
        // continue the loop if the position is not found
        pos++;
        currNode = currNode->next;
    }
    // add the book to the list at the given position
    addBook(list, newBook, pos);
}
/*
  Function:  addByYear 
  Purpose:   add a book by year to the given list
  Parameters: 
	in:      list and book pointer 
	return:  list with books added by year in decending order
*/

void addByYear(ListType *list, BookType *newBook)
{
    NodeType *currNode = list->head;
    int pos = 0;

    // keep looping until end of the list
    while (currNode != NULL)
    {
        //Compare the given book to the book at the current node
        int rc = compareBooksByYear(newBook, currNode->data);

        // the position found where the book needs to be added
        if (rc == 1)
            break;

        // position still not found, keep looping
        pos++;
        currNode = currNode->next;
    }
    // add the book, to to position found
    addBook(list, newBook, pos);
}
/*
  Function:  copyList
  Purpose:   copy all of the nodes and data from the source list to the destination list
  Parameters: 
	in:      source list pointer and a dest list pointer
	return:  the destination list with the all the copied data sorted by acending order by title
*/
void copyList(ListType *src, ListType *dest)
{
    NodeType *currsrc = src->head;

    // while the current list still have data in it
    while (currsrc != NULL)
    {
        // add the book to the dest list
        addByTitle(dest, currsrc->data);
        //continue looping until all the node are copied
        currsrc = currsrc->next;
    }
}
/*
  Function:  copyByYear
  Purpose:   copy data from the source list to the dist list and order it by year
  Parameters: 
	in:      source list pointer and a dest list pointer
	return:  the dest list with the copies data sorted by dcending order by year
*/
void copyByYear(ListType *src, ListType *dest)
{
    NodeType *currsrc;

    currsrc = src->head;

    // while the current list still have data in it
    while (currsrc != NULL)
    {
        // add the book to the dest list
        addByYear(dest, currsrc->data);
        //continue looping until all the node are copied
        currsrc = currsrc->next;
    }
}
/*
  Function:  delAuthor 
  Purpose:   delete a given from a given list
  Parameters: 
	in:     a list and a name pointer
	return: a list 
*/
void delAuthor(ListType *list, char *name)
{

    NodeType *currNode = list->head;
    NodeType *prevNode = NULL;

    while (currNode != NULL)
    {
        int res = strcmp(currNode->data->author, name);

        if (res == 0) // did find author's book that is to be deleted
        {
            NodeType *nodeToBeDeleted = currNode;

            if (prevNode == NULL) // currNode must be the head
                list->head = currNode->next;

            else if (currNode->next == NULL) // currNode must be the tail
            {
                list->tail = prevNode;
                list->tail->next = NULL;
            }
            else // currNode is in the middle of the list
                prevNode->next = currNode->next;

            // select next node to check
            currNode = currNode->next;

            // free the node to be deleted
            free(nodeToBeDeleted);
        }
        else // found the node to be deleted
        {
            prevNode = currNode;
            currNode = currNode->next;
        }
    }
}

/*
  Function:  printList 
  Purpose:   print the data of the given list
  Parameters: 
	in:     list pointer 
	return: printed list
*/
void printList(ListType *list)
{
    NodeType *currNode = list->head;

    printf("BOOK LIST:\n");

    while (currNode != NULL)
    {
        // printf("-----------------------------\n");
        printf(" -- %15s\t", currNode->data->title);
        printf(" by %15s, \t", currNode->data->author);
        printf(" Yr: %d \t \n", currNode->data->yearPub);

        currNode = currNode->next;
    }
    printHead(list);
    printTail(list);
}
/*
  Function:  cleanupList 
  Purpose:   deallocates memory of the nodes of a given list
  Parameters: 
	in:     pointer to a list
	return: -
*/
void cleanupList(ListType *list)
{
    NodeType *currNode = list->head;
    NodeType *next;

    // iterate through the linked list until the end
    while (currNode != NULL)
    {
        next = currNode->next;
        // free the node
        free(currNode);
        // continue iterating
        currNode = next;
    }
}
/*
  Function:  cleanupData 
  Purpose:   deallocate memory of the data of a given list
  Parameters: 
	in:     pointer to a list
	return: -
*/
void cleanupData(ListType *list)
{
    NodeType *currNode, *next;

    currNode = list->head;

    while (currNode != NULL)
    {
        next = currNode->next;
        free(currNode->data);
        currNode = next;
    }
}

/*
  Function:  compareByYear
  Purpose:   compare the year of two booktypes and find which year is lesser, greater or equal
  Parameters: 
	in:     two booktype pointers 
	return: int value 
*/

int compareBooksByYear(BookType *b1, BookType *b2)
{
    // find the yearpub of each book
    int y1 = b1->yearPub;
    int y2 = b2->yearPub;

    // compare the two years of the books
    if (y1 < y2)
        return -1;

    else if (y1 > y2)
        return 1;

    return 0;
}
/*
  Function:  addBook
  Purpose:   adds a given book to a given list in a given position
  Parameters: 
	in:     a list pointer, booktype pointer and int 
	return: list with the added book
*/
void addBook(ListType *list, BookType *newBook, int pos)
{
    NodeType *newNode;
    NodeType *currNode, *prevNode;
    int currPos = 0;

    // create a new node
    newNode = malloc(sizeof(NodeType));
    newNode->data = newBook;
    newNode->next = NULL;

    currNode = list->head;
    prevNode = NULL;

    while (currNode != NULL)
    {
        // if the list it already sorted, break
        if (currPos == pos)
            break;
        prevNode = currNode;
        currNode = currNode->next;
        ++currPos;
    }

    // if the pos is still not found break
    if (currPos != pos)
        return;

    newNode->next = currNode;

    // the node is being added to the head
    if (prevNode == NULL)
        list->head = newNode;
    // node added to the anywhere between the head and tail
    else
        prevNode->next = newNode;
    // the node is being added to the tail
    if (newNode->next == NULL)
        list->tail = newNode;
}
/*
  Function:  printHead 
  Purpose:   print the data of the head of the given list
  Parameters: 
	in:     list pointer 
	return: printed list
*/
void printHead(ListType *list)
{
    printf("--> HEAD is: -- %15s \t", list->head->data->title);
    printf(" by%15s \t", list->head->data->author);
    printf(" Yr: %d \t \n", list->head->data->yearPub);
}
/*
  Function:  printTail
  Purpose:   print the data of the tail of the given list
  Parameters: 
	in:     list pointer 
	return: printed list
*/
void printTail(ListType *list)
{
    printf("--> TAIL is: -- %15s \t", list->tail->data->title);
    printf(" by%15s, \t", list->tail->data->author);
    printf(" Yr: %d \t \n", list->tail->data->yearPub);
}