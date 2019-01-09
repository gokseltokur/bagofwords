/*

Göksel Tokur 150116049

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include<locale.h>
 
struct document{
	char documentID[5];
	int numberOfTimes;
	int length;
	char categoryName[10];
	struct document *nextdocument;
};

struct term{
	char termName[18];
	int numberOfDoc;
	int numberOfT;
	struct term *nextterm;
	struct document *startdocument;
};

typedef struct document document;
typedef struct term term;

int compareTerms(term *a, term *b){
	setlocale(LC_ALL,"turkish");
	return strcmp(a->termName, b->termName);
}

//Function to get length of text.
int getLengthOfTxt(char filename[]){
	int length = 0;
	FILE *file;
	char termName[18];
	if ((file = fopen(filename, "r")) == NULL) {
		printf("File could not be opened in getLengthOfTxt function");
		return 0;
	}else{
		while(fscanf(file, "%s", termName) != EOF){
			length++;
		}
	
		return length;
	}
}

//To read terms in text file and putting these terms in a linked list.
term *readTerms(char filename[], term** startPtr, char documentID[], char categoryName[], int length){
	setlocale(LC_ALL,"turkish");

	//term* sPtr = NULL;
	
	FILE *file;
	
	if ((file = fopen(filename, "r")) == NULL) {
		printf("File could not be opened");
		return NULL;
	}
	else{
		char termName[18];
		while(fscanf(file, "%s", termName) != EOF){
			//printf("%s\n", &termName);
			
			term* newPtr = malloc(sizeof(term));
			if(newPtr != NULL){
				strcpy(newPtr->termName , termName);
				newPtr->nextterm = NULL;
				newPtr->startdocument = NULL;
				newPtr->numberOfDoc = 1;
				newPtr->numberOfT = 1;
				if(*startPtr == NULL || compareTerms(newPtr, *startPtr)<0){
					// adding new node begining of the list
					newPtr->nextterm = *startPtr;
					*startPtr = newPtr;
					newPtr->numberOfDoc++;
					// adding document node
					document* newDoc = malloc(sizeof(document));
					strcpy(newDoc->documentID, documentID);
					strcpy(newDoc->categoryName, categoryName);
					newDoc->length = length;
					newDoc->numberOfTimes = 1;
					newDoc->nextdocument = NULL;
					newPtr->startdocument = newDoc;
					
					
				}else{
					term* currentPtr = *startPtr;
					while(currentPtr->nextterm != NULL && compareTerms(newPtr, currentPtr->nextterm) >= 0){
						// finding correct location
						currentPtr = currentPtr->nextterm;
					}
					if(compareTerms(newPtr, currentPtr) == 0){
						// if new term equals another term do something....
						currentPtr->numberOfT++;
						document* controllerPtr = currentPtr->startdocument;
						int switchh = 0;
						//the case for same txt file same term. // increasing document node's numberoftimes
						/*if(strcmp(controllerPtr->documentID, documentID)==0 && strcmp(controllerPtr->categoryName, categoryName)==0 ){
							controllerPtr->numberOfTimes++;
							switchh = 1 ;
						}*/
						while(controllerPtr != NULL){
							if(strcmp(controllerPtr->documentID, documentID)==0 && strcmp(controllerPtr->categoryName, categoryName)==0 ){
								controllerPtr->numberOfTimes++;
								switchh = 1;
							}
							controllerPtr = controllerPtr->nextdocument;
						}
						
						// the case for different txt file same term. // adding new document node
						document* controllerPtr2 = currentPtr->startdocument;
						while(controllerPtr2->nextdocument != NULL){
							controllerPtr2 = controllerPtr2->nextdocument;
						}
						if(switchh == 0){
							currentPtr->numberOfDoc++;
							document* newDoc = malloc(sizeof(document));
							strcpy(newDoc->documentID, documentID);
							strcpy(newDoc->categoryName, categoryName);
							newDoc->length = length;
							newDoc->numberOfTimes = 1;
							newDoc->nextdocument = NULL;
							controllerPtr2->nextdocument = newDoc;
						}
						
						
						free(newPtr);
						
						
					}else{
						// adding new node
						newPtr->nextterm = currentPtr->nextterm;
						currentPtr->nextterm = newPtr;
						
						//adding document node
						document* newDoc = malloc(sizeof(document));
						strcpy(newDoc->documentID, documentID);
						strcpy(newDoc->categoryName, categoryName);
						newDoc->length = length;
						newDoc->numberOfTimes = 1;
						newDoc->nextdocument = NULL;
						newPtr->startdocument = newDoc;
					}
				}
			}
		}
	}
}

void printTerms(term* termptr){
	while (termptr != NULL) { 
        printf("%s %d---", termptr->termName, termptr->numberOfT);
        document* docptr = termptr->startdocument;
    
        while(docptr != NULL){
        	printf("%s %s %d %d ///", docptr->categoryName, docptr->documentID, docptr->length, docptr->numberOfTimes);
        	docptr = docptr->nextdocument;
    	}
    	printf("\n");
        termptr = termptr->nextterm;   
    }
}

void swap(term* a, term* b){ 
	
    char tempTermName[18];
    strcpy(tempTermName, a->termName);
    int tempNumberOfDoc = a->numberOfDoc;
    int tempNumberOfT = a->numberOfT;
    document *tempStartDocument = a->startdocument;
    
    //a->data = b->data;
    strcpy(a->termName, b->termName);
	a->numberOfDoc = b->numberOfDoc; 
	a->numberOfT = b->numberOfT; 
	a->startdocument = b->startdocument; 
    
    //b->data = temp; 
    strcpy(b->termName, tempTermName);
    b->numberOfDoc = tempNumberOfDoc;
    b->numberOfT = tempNumberOfT;
    b->startdocument = tempStartDocument;
    
} 

/* Bubble sort the given linked list */
void sortByNumberOfT(term* start) 
{ 
    int swapped, i; 
    term* ptr1; 
    term* lptr = NULL; 
  
    /* Checking for empty list */
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->nextterm != lptr) 
        { 
            if (ptr1->numberOfT < ptr1->nextterm->numberOfT) 
            {  
                swap(ptr1, ptr1->nextterm); 
                swapped = 1; 
            } 
            ptr1 = ptr1->nextterm; 
        } 
        lptr = ptr1; 
    } 
    while (swapped);
    
    
}

//Sorting the linked list by ascending order
void sortByAscendingOrder(term* start, char category1Name[], char category2Name[], char category3Name[]){
	// finding first five node and sort this five node by ascending order
	term* ptrx = start;
    int y;
    int category1, category2, category3;
	category1 = 0;
	category2 = 0;
	category3 = 0;
    for(y = 0; y<4;){
    	document* docptr = ptrx->startdocument;
		while(docptr != NULL){
			if(strcmp(docptr->categoryName, category1Name) == 0 )
				category1 = 1;
			if(strcmp(docptr->categoryName, category2Name) == 0 )
				category2 = 1;
			if(strcmp(docptr->categoryName, category3Name) == 0 )
				category3 = 1;
			docptr = docptr->nextdocument;
		}
    	if(category1 == 1 && category2 == 1 && category3 == 1)
			y++;
    	ptrx = ptrx->nextterm;
	}
	ptrx->nextterm = NULL;
	
	
	
	int swapped, i; 
    term* ptr1; 
    term* lptr = NULL; 
  
    /* Checking for empty list */
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->nextterm != lptr) 
        { 
            if (compareTerms(ptr1, ptr1->nextterm) > 0) 
            {  
                swap(ptr1, ptr1->nextterm); 
                swapped = 1; 
            } 
            ptr1 = ptr1->nextterm; 
        } 
        lptr = ptr1; 
    } 
    while (swapped);
	
}

// Print first five term that occur in all categories.
void printb(term* termptr, char category1Name[], char category2Name[], char category3Name[]){
	
	int firstfive = 1;
	while (termptr != NULL) {
		int category1, category2, category3;
		category1 = 0;
		category2 = 0;
		category3 = 0;
		
		document* docptr = termptr->startdocument;
		while(docptr != NULL){		
			if(strcmp(docptr->categoryName, category1Name) == 0 )
				category1 = 1;
			if(strcmp(docptr->categoryName, category2Name) == 0 )
				category2 = 1;
			if(strcmp(docptr->categoryName, category3Name) == 0 )
				category3 = 1;		
			
			docptr = docptr->nextdocument;
		}
		if(category1 == 1 && category2 == 1 && category3 == 1){
			printf("Term-%d: %s\n", firstfive, termptr->termName );
			firstfive++;
		}
		if(firstfive == 6)
			break;
		termptr = termptr->nextterm;	
	}
	printf("\n");
}

// Insert function for the linkedlist.
term* add(term** headptr, char termName[18]){

	//create new node
    term* newNode = (term*)malloc(sizeof(term));
	if(newNode != NULL){
	
	    strcpy(newNode->termName, termName);
	    newNode->numberOfDoc = 0;
	    newNode->numberOfT = 0;
	    newNode->startdocument = NULL;
	    newNode->nextterm = NULL; 

	    if(*headptr == NULL){
	    	
	        newNode->nextterm = *headptr;
			*headptr = newNode;
	    }else{
	    	
	        term* currentptr = *headptr;
			while(currentptr->nextterm != NULL){
				// finding correct location
				currentptr = currentptr->nextterm;
			}
			currentptr->nextterm = newNode;
			newNode->nextterm = NULL;
	    }
	}

}

//Print first five term category by category
void printc(term* termptr, char category1Name[], char category2Name[], char category3Name[]){
	
	term* category1 = NULL;
	term* category2 = NULL;
	term* category3 = NULL;
	
	while(termptr != NULL){
		
		document* docptr = termptr->startdocument;
		char tempDocName[10];
		strcpy(tempDocName, docptr->categoryName);
		int controller = 1;
		docptr = docptr->nextdocument;
		while(docptr != NULL){
			if(strcmp(docptr->categoryName, tempDocName) != 0){
				controller = 0;
			}
			
			docptr = docptr->nextdocument;
		}
		
		if(controller == 1 && strcmp(tempDocName, category1Name) == 0)
			add(&category1, termptr->termName);
		if(controller == 1 && strcmp(tempDocName, category2Name) == 0)
			add(&category2, termptr->termName);
		if(controller == 1 && strcmp(tempDocName, category3Name) == 0)
			add(&category3, termptr->termName);	
	
		termptr = termptr->nextterm;
		
	}
	
	int i;
	printf("%-18s%-18s%-18s%\n", "Category-1", "Category-2", "Category-3");
	printf("---------------------------------------------------\n");
	for(i = 0 ; i<5; i++){
		printf("%-18s%-18s%-18s%\n", category1->termName, category2->termName, category3->termName);
		category1 = category1->nextterm;
		category2 = category2->nextterm;
		category3 = category3->nextterm;
	}
}
 

int main(){
	DIR* FD;
    struct dirent* in_file;
    FILE    *common_file;
    FILE    *entry_file;
    char    buffer[BUFSIZ];
    
    DIR* FDx;
    struct dirent* in_filex;
    FILE    *common_filex;
    FILE    *entry_filex;
    char    bufferx[BUFSIZ];
    
	term* ascendingOrdered = NULL;
	term* numberOfTOrdered = NULL;
	char category1Name[20];
	char category2Name[20];
	char category3Name[20];
	int categoryNameController = 1; // controller to take category names seperately
	

    /* Openiing common file for writing */
    common_file = fopen("r.txt", "w");
    if (common_file == NULL)
    {
        fprintf(stderr, "Error : Failed to open common_file - %s\n", strerror(errno));

        return 1;
    }
	
	if (NULL == (FDx = opendir (".\\dataset"))) 
    {
        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
        fclose(common_file);

        return 1;
    }
    
    // in the category ********************
	while ((in_filex = readdir(FDx))){
		if (!strcmp (in_filex->d_name, "."))
            continue;
        if (!strcmp (in_filex->d_name, ".."))    
            continue;
        
        
		char path1[50]= ".\\dataset\\";
		strcat(path1, in_filex->d_name);
		
		//to take category names.................
		if(categoryNameController == 1)
			strcpy(category1Name, in_filex->d_name);
		
		if(categoryNameController == 2)
			strcpy(category2Name, in_filex->d_name);
		
		if(categoryNameController == 3)
			strcpy(category3Name, in_filex->d_name);
		
		
		//printf("%s\n", path1);
	    /* Scanning the in directory */
	    if (NULL == (FD = opendir (path1))) 
	    {
	        fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
	        fclose(common_file);
	
	        return 1;
	    }
	    while ((in_file = readdir(FD))) 
	    {
	      
	        if (!strcmp (in_file->d_name, "."))
	            continue;
	        if (!strcmp (in_file->d_name, ".."))    
	            continue;
	        /* Open directory entry file for common operation */
	        /* TODO : change permissions to meet your need! */
	        //char path[50] = "dataset\\econ\\" ;
	        char path1[50]= ".\\dataset\\";
			strcat(path1, in_filex->d_name);
	        strcat(path1, "\\" );
	        strcat(path1, in_file->d_name );
	        //printf("%s\n", path1);
	        entry_file = fopen( path1, "rw");
	        //sending category name, document id and lenght of txt file to the readTerms function.
			char categoryName[10];
	        char documentID[5];
	        strcpy(categoryName, in_filex->d_name);
	        strcpy(documentID, in_file->d_name);
	        int length = getLengthOfTxt(path1);
	        //printf("%d\n", length);
	        readTerms(path1, &ascendingOrdered, documentID, categoryName, length);
	        readTerms(path1, &numberOfTOrdered, documentID, categoryName, length);
	        
	        
	        
	        if (entry_file == NULL)
	        {
	            fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
	            fclose(common_file);
	
	            return 1;
	        }
	        
	        while (fgets(buffer, BUFSIZ, entry_file) != NULL)
	        {	    
	            /* Use fprintf or fwrite to write some stuff into txt */
	        }
	        fclose(entry_file);
	    }
	    categoryNameController++;
	}
	
	/*************** Activate this code to print linkedlist on the console. *************** */
	//printTerms(ascendingOrdered); 
	
	sortByNumberOfT(numberOfTOrdered);
	sortByAscendingOrder(numberOfTOrdered, category1Name, category2Name, category3Name);
	printb(numberOfTOrdered, category1Name, category2Name, category3Name);
	printc(ascendingOrdered, category1Name, category2Name, category3Name);


    fclose(common_file);

    return 0;
}


