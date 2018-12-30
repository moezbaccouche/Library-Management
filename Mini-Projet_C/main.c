#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************** Structs related to BOOKS ******************************/

typedef struct Book Book;
struct Book
{
	char isbn[5];
	char title[20];
	int nbBooks;
	char linkPdf[256];
};

typedef struct ElemBook ElemBook;
struct ElemBook
{
	Book book;
	ElemBook *next;
};

typedef ElemBook *ListBooks;

/***************************************************************************************/

/****************************** Structs related to LOANS ******************************/
typedef struct Loan Loan;
struct Loan
{
	int numLoan;
	char NCE[9];
	char ISBN[5];
	char dateLoan[11];
	char dateReturn[11];
};

typedef struct ElemLoan ElemLoan;
struct ElemLoan
{
	Loan loan;
	ElemLoan *next;
};

typedef ElemLoan *ListLoans;

/***************************************************************************************/
int isValidDate(char*);
void removeChar(char*, char);
void concatChar(char*, char);
void displayMainMenu(ListBooks, ListLoans);
void displayLoanMenu(ListLoans, ListBooks);
void displayBookMenu(ListBooks, ListLoans);

void exitApp(ListBooks, ListLoans);
/***************************** Functions prototypes related to BOOKS *****************************/


void displayListBooks(ListBooks);
ListBooks deleteBookFromList(ListBooks, char*);
ListBooks findBookByTitle(ListBooks, char*);
ListBooks findBookByIsbn(ListBooks, char*);
ListBooks editBook(ListBooks, char*, Book);
Book createBook(ListBooks);
void deleteBookMenu(ListBooks, ListLoans);
void editBookMenu(ListBooks, ListLoans);
void returnToBookMenu(ListBooks, ListLoans);
void setPdfLink(char*, char*);
void decrementNbBooks(ListBooks, char*);
void incrementNbBooks(ListBooks, char*);
void specificBookMenu(ListBooks, ListLoans);


ListBooks initListBooksFromFile(FILE*, ListBooks);
void insertListBooksIntoFile(ListBooks);


/*************************************************************************************************/






/***************************** Functions prototypes related to LOANS *****************************/

ListLoans addNewLoanToList(ListLoans, ListBooks, Loan);
void returnToLoanMenu(ListLoans, ListBooks);
Loan createLoan(ListLoans, ListBooks);
void displayLoansList(ListLoans);
int getListLength(ListLoans);
ListLoans findLoanByNum(ListLoans, int);
ListLoans editLoan(ListLoans, int, Loan);
void editLoanMenu(ListLoans, ListBooks);
ListLoans deleteLoanFromList(ListLoans, int, ListBooks);
void deleteLoanMenu(ListLoans, ListBooks);

ListLoans initListLoansFromFile(FILE*, ListLoans, ListBooks);
void insertListLoansIntoFile(ListLoans);
int getNbSpacesForNumLoan(int);

ListLoans addNewStudentLoanToList(ListLoans, Loan);
ListLoans findLoansByNCE(ListLoans, char*);
void studentLoansMenu(ListLoans, ListBooks);


/*************************************************************************************************/









/***************************** Functions implementations related to BOOKS *****************************/

//Add new Book at the end of the List
ListBooks addNewBookToList(ListBooks list, Book book)
{
	ElemBook *newBook = malloc(sizeof(ElemBook));
	if (newBook == NULL)
	{
		printf("Erreur Memoire !");
		exit(0);
	}

	newBook->book = book;
	newBook->next = NULL;

	if (list == NULL)
	{
		printf("\nLivre Ajoute avec succes !\n\n");
		return newBook;
	}
	else
	{
		ElemBook *bTmp = list;
		while (bTmp->next != NULL)
		{
			bTmp = bTmp->next;
		}
		bTmp->next = newBook;
	}

	printf("\nLivre Ajoute avec succes !\n\n");
	return list;
}


void displayListBooks(ListBooks list)
{
	ElemBook *b = list;
	if (list == NULL)
	{
		printf("\nAucun livre disponible !\n");
	}
	else
	{
		printf("\nLes livres disponibles :\n\n");
		while (b != NULL)
		{
			printf("ISBN: %s\n", b->book.isbn);
			printf("Titre: %s\n", b->book.title);
			printf("Nombre d\'exemplaires: %d\n", b->book.nbBooks);
			printf("Chemin PDF: %s\n", b->book.linkPdf);

			printf("\n\n");

			b = b->next;
		}
	}
}

ListBooks deleteBookFromList(ListBooks list, char* isbn)
{
	ElemBook *bTmp = list;
	ElemBook *bPred = list;
	ElemBook *bTmp2 = list;

	if (list == NULL)
	{
		printf("Aucun livre trouve !\n");
		return NULL;
	}

	//The element to delete is the first element of the list
	if (strcmp(list->book.isbn, isbn) == 0)
	{
		bTmp = list;
		list = bTmp->next;
		free(bTmp);

		printf("\nLivre supprime avec succes !\n\n");
		return list;
	}
	else
	{
		bTmp = list->next;
		while (bTmp != NULL)
		{
			if (strcmp(bTmp->book.isbn, isbn) == 0)
			{
				bTmp2 = bTmp->next;
				bPred->next = bTmp2;
				free(bTmp);

				printf("\nLivre supprime avec succes !\n\n");
				return list;
			}
			bTmp = bTmp->next;
			bPred = bPred->next;
		}
	}

	printf("Aucun livre ne correspond a l\'ISBN saisi !\n");
	return list;
}

ListBooks findBookByTitle(ListBooks list, char* title)
{
	ElemBook *bTmp = list;
	while (bTmp != NULL)
	{
		if (strcmp(bTmp->book.title, title) == 0)
		{
			return bTmp;
		}
		bTmp = bTmp->next;
	}

	return NULL;
}

ListBooks findBookByIsbn(ListBooks list, char* isbn)
{
	ElemBook *bTmp = list;
	while (bTmp != NULL)
	{
		if (strcmp(bTmp->book.isbn, isbn) == 0)
		{
			return bTmp;
		}
		bTmp = bTmp->next;
	}

	return NULL;
}

ListBooks editBook(ListBooks list, char* isbn, Book editedBook)
{
	ElemBook *bTmp = list;
	while (bTmp != NULL)
	{
		if (strcmp(bTmp->book.isbn, isbn) == 0)
		{
			bTmp->book = editedBook;

			printf("\nLivre modifie avec succes !\n");
			return list;
		}
		bTmp = bTmp->next;
	}

	printf("Le livre a modifier n\'existe pas !\n");
	return NULL;
}


Book createBook(ListBooks list)
{
	Book book;
	char linkPdf[200];

	int alreadyExists = 0;


	printf("\n=============== Informations livre ===============\n\n");

	do
	{
		alreadyExists = 0;

		printf("ISBN: ");
		do
		{
			fgets(book.isbn, 256, stdin);
			removeChar(book.isbn, '\n');
		} while (strlen(book.isbn) != 4);
		/*scanf("%[^\n]s", book.isbn);*/

		if (findBookByIsbn(list, book.isbn) != NULL)
		{
			printf("Ce livre existe deja, verifiez l\'ISBN saisi !\n");
			alreadyExists = 1;
		}
		else
		{
			alreadyExists = 0;
		}


	} while ((strlen(book.isbn) != 4) || (alreadyExists == 1));

	do
	{
		printf("Titre: ");

		fgets(book.title, 256, stdin);
		removeChar(book.title, '\n');
	} while (strlen(book.title) < 4);

	do
	{
		printf("Nombre d\'exemplaires: ");
		scanf("%d", &book.nbBooks);
	} while (book.nbBooks < 1 || book.nbBooks > 9);

	printf("\n");

	setPdfLink(book.title, linkPdf);

	strcpy(book.linkPdf, linkPdf);

	return book;

	//Missing controle on total books input
}

void editBookMenu(ListBooks listBooks, ListLoans listLoans)
{
	char isbn[5];
	char choice[2];
	Book editedBook;

	char newTitle[256];
	int newNbBooks;
	char linkPdf[200];


	do
	{
		printf("ISBN du livre a modifier (q pour quitter): ");
		do
		{
			fgets(isbn, 5, stdin);
			removeChar(isbn, '\n');
		} while (strlen(isbn) == 0);
	} while ((findBookByIsbn(listBooks, isbn) == NULL) && (strcmp(isbn, "q") != 0));

	if (strcmp(isbn, "q") == 0)
	{
		displayBookMenu(listBooks, listLoans);
	}
	else
	{
		printf("\n========================================================================================================================\n\n");
		printf("   MENU MODIFICATION LIVRE\n");
		printf("     > 1.  Modifier le titre\n");
		printf("     > 2.  Modifier le nombre d\'exemplaires\n");
		printf("     > 0.  Retour au menu gestion des livres\n\n");
		printf("========================================================================================================================\n");

		do
		{
			printf("Choix: ");
			do
			{
				fgets(choice, 2, stdin);
				removeChar(choice, '\n');
			} while (strlen(choice) == 0);
		} while ((strcmp(choice, "1") != 0) && (strcmp(choice, "2") != 0) && (strcmp(choice, "0") != 0));


		ListBooks currentBook = findBookByIsbn(listBooks, isbn);

		if (strcmp(choice, "1") == 0)
		{

			printf("Ancien titre: %s\n", currentBook->book.title);
			do
			{
				printf("Nouveau titre: ");
				do
				{
					fgets(newTitle, 256, stdin);
					removeChar(newTitle, '\n');
				} while (strlen(newTitle) == 0);

			} while (strlen(editedBook.title) < 4);
			strcpy(editedBook.title, newTitle);

			setPdfLink(newTitle, linkPdf);
			strcpy(editedBook.linkPdf, linkPdf);

			strcpy(editedBook.isbn, isbn);

			editedBook.nbBooks = currentBook->book.nbBooks;

			listBooks = editBook(listBooks, isbn, editedBook);

			returnToBookMenu(listBooks, listLoans);
		}

		if (strcmp(choice, "2") == 0)
		{
			printf("Ancien nombre d\'exemplaires: %d\n", currentBook->book.nbBooks);
			do
			{
				printf("Nouveau nombre d\'exemplaires: ");
				scanf("%d", &newNbBooks);

			} while (newNbBooks < 1);
			strcpy(editedBook.isbn, isbn);

			strcpy(editedBook.title, currentBook->book.title);

			strcpy(editedBook.linkPdf, currentBook->book.linkPdf);

			editedBook.nbBooks = newNbBooks;

			listBooks = editBook(listBooks, isbn, editedBook);

			returnToBookMenu(listBooks, listLoans);
		}

		if (strcmp(choice, "0") == 0)
		{
			displayBookMenu(listBooks, listLoans);
		}
	}
}

void deleteBookMenu(ListBooks listBooks, ListLoans listLoans)
{
	char isbn[5];

	char confirm[2];

	do
	{
		printf("ISBN du livre a supprimer (q pour quitter): ");
		do
		{
			fgets(isbn, 5, stdin);
			removeChar(isbn, '\n');
		} while (strlen(isbn) == 0);
	} while ((strcmp(isbn, "q") != 0) && (findBookByIsbn(listBooks, isbn) == NULL));

	if (strcmp(isbn, "q") == 0)
	{
		displayBookMenu(listBooks, listLoans);
	}
	else
	{
		do
		{
			printf("\nEtes-vous sur de vouloir supprimer ce livre ? (o/n)  ");

			do
			{
				fgets(confirm, 2, stdin);
				removeChar(confirm, '\n');
			} while (strlen(confirm) == 0);


		} while (strcmp(confirm, "o") != 0 && strcmp(confirm, "n") != 0);

		if (strcmp(confirm, "o") == 0)
		{
			listBooks = deleteBookFromList(listBooks, isbn);
			returnToBookMenu(listBooks, listLoans);
		}
		else
		{
			printf("\nSuppression annulee !\n");
			returnToBookMenu(listBooks, listLoans);
		}
	}
}

void specificBookMenu(ListBooks listBooks, ListLoans listLoans)
{
	char isbn[5];

	int notFound = 0;
	char choice[2];


	do
	{
		if (notFound == 1)
		{
			printf("\nL\'ISBN saisi ne correspond a aucun livre !\n");
		}
		do
		{
			printf("ISBN du livre a afficher (q pour quitter): ");
			do
			{
				fgets(isbn, 5, stdin);
				removeChar(isbn, '\n');
			} while (strlen(isbn) == 0);
		} while ((strlen(isbn) != 4) && (strcmp(isbn, "q") != 0));

		if (strcmp(isbn, "q") == 0)
		{
			displayBookMenu(listBooks, listLoans);
		}
		else
		{
			ListBooks searchedBook = findBookByIsbn(listBooks, isbn);
			if (searchedBook == NULL)
			{
				notFound = 1;
			}
			else
			{
				notFound = 0;
			}
		}
	} while (notFound == 1 && strcmp(isbn, "q") != 0);


	if (strcmp(isbn, "q") == 0)
	{
		displayBookMenu(listBooks, listLoans);
	}
	else
	{
		ListBooks searchedBook = findBookByIsbn(listBooks, isbn);
		printf("\n=============== Informations livre recherche ==========\n\n");
		printf("ISBN: %s\n", searchedBook->book.isbn);
		printf("Titre: %s\n", searchedBook->book.title);
		printf("Nombre d\'exemplaires: %d\n", searchedBook->book.nbBooks);
		printf("Lien PDF: %s\n\n", searchedBook->book.linkPdf);

		do
		{
			printf("Appuyez sur p pour afficher le fichier PDF ou q pour retourner au menu GESTIONS DES LIVRES. ");
			do
			{
				fgets(choice, 2, stdin);
				removeChar(choice, '\n');
			} while (strlen(choice) == 0);
		} while (strcmp(choice, "q") != 0 && strcmp(choice, "p") != 0);

		if (strcmp(choice, "q") == 0)
		{
			displayBookMenu(listBooks, listLoans);
		}
		else
		{
			printf("\n");
			system(searchedBook->book.linkPdf);
			returnToBookMenu(listBooks, listLoans);
		}

	}
}

void displayBookMenu(ListBooks listBooks, ListLoans listLoans)
{
	char choice[2];
	int i, nbBooksToAdd;


	printf("\n\n");
	printf("                                      ============================================\n");
	printf("                                      ====                                    ====\n");
	printf("                                      ====            EPI - BIBLIO            ====\n");
	printf("                                      ====         GESTION DES LIVRES         ====\n");
	printf("                                      ====                                    ====\n");
	printf("                                      ============================================\n\n");

	printf("========================================================================================================================\n\n");
	printf("   MENU GESTION DES LIVRES\n");
	printf("     > 1.  Ajouter un livre\n");
	printf("     > 2.  Modifier un livre\n");
	printf("     > 3.  Supprimer un livre\n");
	printf("     > 4.  Afficher tous les livres\n");
	printf("     > 5.  Afficher un livre specifique\n");
	printf("     > 0.  Retour au menu principal\n");
	printf("\n");
	printf("========================================================================================================================\n\n");

	do
	{
		printf("Choix : ");
		do
		{
			fgets(choice, 2, stdin);
			removeChar(choice, '\n');
		} while (strlen(choice) == 0);
	} while ((strcmp(choice, "1") != 0) && (strcmp(choice, "2") != 0) && (strcmp(choice, "3") != 0) && (strcmp(choice, "4") != 0) && (strcmp(choice, "5") != 0) && (strcmp(choice, "0") != 0));

	if (strcmp(choice, "1") == 0)
	{
		do
		{
			printf("\nCombien de livres voulez-vous ajouter ? ");
			scanf("%d", &nbBooksToAdd);
		} while (nbBooksToAdd < 1);


		for (i = 0; i < nbBooksToAdd; i++)
		{
			Book book = createBook(listBooks);
			listBooks = addNewBookToList(listBooks, book);
		}
		returnToBookMenu(listBooks, listLoans);
	}

	if (strcmp(choice, "2") == 0)
	{
		editBookMenu(listBooks, listLoans);

		returnToBookMenu(listBooks, listLoans);
	}

	if (strcmp(choice, "3") == 0)
	{
		deleteBookMenu(listBooks, listLoans);

		returnToBookMenu(listBooks, listLoans);
	}

	if (strcmp(choice, "4") == 0)
	{
		displayListBooks(listBooks);

		returnToBookMenu(listBooks, listLoans);
	}

	if (strcmp(choice, "5") == 0)
	{
		specificBookMenu(listBooks, listLoans);

		returnToBookMenu(listBooks, listLoans);
	}

	if (strcmp(choice, "0") == 0)
	{
		displayMainMenu(listBooks, listLoans);
	}

}


void displayMainMenu(ListBooks listBooks, ListLoans listLoans)
{
	char choice[2];

	printf("\n\n");
	printf("                                      ============================================\n");
	printf("                                      ====                                    ====\n");
	printf("                                      ====         EPI - BIBLIOTHEQUE         ====\n");
	printf("                                      ====                                    ====\n");
	printf("                                      ============================================\n\n");

	printf("========================================================================================================================\n\n");
	printf("   MENU PRINCIPAL\n");
	printf("     > 1.  Gestion des livres\n");
	printf("     > 2.  Gestion des emprunts\n");
	printf("     > 0.  Quitter\n");
	printf("\n");
	printf("========================================================================================================================\n\n");


	do
	{
		printf("Choix : ");
		do
		{
			fgets(choice, 2, stdin);
			removeChar(choice, '\n');
		} while (strlen(choice) == 0);

	} while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "0") != 0);

	if (strcmp(choice, "1") == 0)
	{
		displayBookMenu(listBooks, listLoans);
	}
	else
	{
		if (strcmp(choice, "2") == 0)
		{
			displayLoanMenu(listLoans, listBooks);
		}
		else
		{
			exitApp(listBooks, listLoans);
		}
	}
}

void returnToBookMenu(ListBooks listBooks, ListLoans listLoans)
{
	char retBookMenu[2];

	do
	{
		printf("Appuyer sur q pour retourner au menu de gestion des livres.  ");
		do
		{
			fgets(retBookMenu, 2, stdin);
			removeChar(retBookMenu, '\n');
		} while (strlen(retBookMenu) == 0);
	} while (strcmp(retBookMenu, "q") != 0);


	displayBookMenu(listBooks, listLoans);
}

void removeChar(char *string, char toDelete)
{

	char *src, *dst;
	for (src = dst = string; *src != '\0'; src++)
	{
		*dst = *src;
		if (*dst != toDelete)
		{
			dst++;
		}
	}
	*dst = '\0';
}

void setPdfLink(char* title, char* linkPdf)
{
	char link[200];
	char linkTitle[200];

	strcpy(link, ".\\PDF\\");
	strcat(link, title);
	strcpy(linkTitle, link);
	strcat(linkTitle, ".pdf");

	removeChar(linkTitle, ' ');
	strcpy(linkPdf, linkTitle);

}


void decrementNbBooks(ListBooks listBooks, char *isbn)
{
	ElemBook *bTmp = listBooks;
	while (bTmp != NULL)
	{
		if (strcmp(bTmp->book.isbn, isbn) == 0)
		{
			bTmp->book.nbBooks--;
			bTmp = NULL;
		}
		else
		{
			bTmp = bTmp->next;
		}
	}
}

void incrementNbBooks(ListBooks listBooks, char *isbn)
{
	ElemBook *bTmp = listBooks;
	while (bTmp != NULL)
	{
		if (strcmp(bTmp->book.isbn, isbn) == 0)
		{
			bTmp->book.nbBooks++;
			bTmp = NULL;
		}
		else
		{
			bTmp = bTmp->next;
		}
	}
}


ListBooks initListBooksFromFile(FILE *file, ListBooks listBooks)
{
	char line[1000];
	Book newBook;
	int i = 0;

	char isbn[5] = "";

	char title[256] = "";
	int nbBooks;
	char pdfLink[256] = "";
	char nbBooksString[3] = "";


	file = fopen("./Files/Books.txt", "r");
	if (file == NULL)
	{
		printf("\nLe fichier n\'existe pas !\n");
	}
	else
	{
		fseek(file, 164, SEEK_SET);
		while (fgets(line, 1000, file) != NULL)
		{

			for (i = 0; i < 4; i++)
			{
				concatChar(isbn, line[i]);
			}
			

			strcpy(newBook.isbn, isbn);
			strcpy(isbn, "");

			for (i = 16; i < 34; i++)
			{
				if (line[i] == ' ' && line[i + 1] == ' ')
				{
					break;
				}
				concatChar(title, line[i]);
			}
			

			strcpy(newBook.title, title);
			strcpy(title, "");


			for (i = 40; i < 43; i++)
			{
				if (line[i] == ' ' && line[i + 1] == ' ')
				{
					break;
				}
				concatChar(nbBooksString, line[i]);
			}
			nbBooks = atoi(nbBooksString);
			strcpy(nbBooksString, "");

			newBook.nbBooks = nbBooks;
			
			

			for (i = 64; i < 100; i++)
			{
				if (line[i] == '\0')
				{
					break;
				}
				if (line[i] == ' ' && line[i + 1] == ' ')
				{
					break;
				}
				concatChar(pdfLink, line[i]);
			}
			


			strcpy(newBook.linkPdf, pdfLink);
			strcpy(pdfLink, "");

			listBooks = addNewBookToList(listBooks, newBook);

		}

		fclose(file);
	}

	return listBooks;
}


void insertListBooksIntoFile(ListBooks listBooks)
{
	int nbSpaces = 0;
	int titleLen;
	int i = 0;

	FILE *file = fopen("./Files/books.txt", "w");
	if (file == NULL)
	{
		printf("\nErreur lors de l'ouverture du fichier !\n");
	}
	else
	{
		fprintf(file, "ISBN            TITRE                   QUANTITE                CHEMIN\n");
		fprintf(file, "------------------------------------------------------------------------------------------\n");

		ElemBook *bTmp = listBooks;
		while (bTmp != NULL)
		{
			titleLen = strlen(bTmp->book.title);

			fprintf(file, "%s            ", bTmp->book.isbn);
			fprintf(file, "%s", bTmp->book.title);
			for (i = 0; i < 24 - titleLen; i++)
			{
				fprintf(file, " ");
			}
			fprintf(file, "%d                       ", bTmp->book.nbBooks);

			removeChar(bTmp->book.linkPdf, '\n');
			fprintf(file, "%s\n", bTmp->book.linkPdf);

			bTmp = bTmp->next;
		}
	}
}

void exitApp(ListBooks listBooks, ListLoans listLoans)
{
	insertListBooksIntoFile(listBooks);
	insertListLoansIntoFile(listLoans);

	printf("\n\n\nAu revoir !\n\n");
	exit(0);
}

void concatChar(char* string, char c)
{
	int i = strlen(string);
	string[i] = c;
	string[i + 1] = '\0';
}



/*************************************************************************************************/






/***************************** Implementations of functions related to LOANS *****************************/

Loan createLoan(ListLoans listLoans, ListBooks listBooks)
{
	char nce[9];
	char isbnInput[5];
	char dateLoan[11];
	char dateReturn[11];
	int notFound = 0;
	int unavailable = 0;
	Loan newLoan;

	int valid;

	do
	{
		if (notFound == 1)
		{
			printf("\nLe livre recherche n\'existe pas, verifiez l\'ISBN saisi !\n");
		}
		do
		{
			printf("ISBN du livre a emprunter (q pour quitter): ");
			do
			{
				fgets(isbnInput, 5, stdin);
				removeChar(isbnInput, '\n');
			} while (strlen(isbnInput) == 0);
		} while ((strlen(isbnInput) != 4) && (strcmp(isbnInput, "q") != 0));

		if (strcmp(isbnInput, "q") == 0)
		{
			displayLoanMenu(listLoans, listBooks);
		}
		else
		{
			ListBooks foundBook = findBookByIsbn(listBooks, isbnInput);
			if (foundBook == NULL)
			{
				notFound = 1;
			}
			else
			{
				if (foundBook->book.nbBooks == 0)
				{
					unavailable = 1;
					printf("\nStock epuise, il n y a plus d\'exemplaires pour ce livre !\n");
				}
				notFound = 0;
			}
		}
	} while (notFound == 1 || unavailable == 1);


	printf("\n\n=============== Informations Emprunt ===============\n\n");
	do
	{
		printf("NCE: ");
		do
		{
			fgets(nce, 9, stdin);
			removeChar(nce, '\n');
		} while (strlen(nce) == 0);
	} while (strlen(nce) != 8);

	strcpy(newLoan.NCE, nce);

	do
	{
		printf("Date d\'emprunt: ");
		do
		{
			fgets(dateLoan, 11, stdin);
			removeChar(dateLoan, '\n');
		} while (strlen(dateLoan) == 0);

		valid = isValidDate(dateLoan);

	} while (strlen(dateLoan) != 10 || valid != 1);

	strcpy(newLoan.dateLoan, dateLoan);

	do
	{
		printf("Date de retour: ");
		do
		{
			fgets(dateReturn, 11, stdin);
			removeChar(dateReturn, '\n');
		} while (strlen(dateReturn) == 0);

		valid = isValidDate(dateLoan);

	} while (strlen(dateReturn) != 10 || valid != 1);

	strcpy(newLoan.dateReturn, dateReturn);

	strcpy(newLoan.ISBN, isbnInput);

	newLoan.numLoan = getListLength(listLoans) + 1;

	return newLoan;
}


ListLoans addNewLoanToList(ListLoans listLoans, ListBooks listBooks, Loan loan, int fromFile)
{
	ElemLoan *newLoan = malloc(sizeof(ElemLoan));
	if (newLoan == NULL)
	{
		printf("Erreur Memoire !");
		exit(0);
	}

	newLoan->loan = loan;
	newLoan->next = NULL;

	if (listLoans == NULL)
	{
		if (fromFile == 0)
		{
			decrementNbBooks(listBooks, loan.ISBN);
		}
		
		printf("\nEmprunt enregistre avec succes !\n\n");
		return newLoan;
	}
	else
	{
		ElemLoan *lTmp = listLoans;
		while (lTmp->next != NULL)
		{
			lTmp = lTmp->next;
		}
		lTmp->next = newLoan;
	}

	if (fromFile == 0)
	{
		decrementNbBooks(listBooks, loan.ISBN);
	}
	printf("\nEmprunt enregistre avec succes !\n\n");
	return listLoans;
}

ListLoans findLoanByNum(ListLoans list, int numLoan)
{
	ElemLoan *lTmp = list;
	while (lTmp != NULL)
	{
		if (lTmp->loan.numLoan == numLoan)
		{
			return lTmp;
		}
		lTmp = lTmp->next;
	}

	return NULL;
}

void editLoanMenu(ListLoans listLoans, ListBooks listBooks)
{
	char answer[3];
	int numLoan;
	char choice[2];
	Loan editedLoan;

	char newLoanDate[11];
	char newReturnDate[11];

	int valid;


	do
	{
		printf("Numero de l\'emprunt a modifier (q pour quitter): ");
		do
		{
			fgets(answer, 3, stdin);
			removeChar(answer, '\n');
		} while (strlen(answer) == 0);
	} while ((findLoanByNum(listLoans, atoi(answer)) == NULL) && (strcmp(answer, "q") != 0));



	if (strcmp(answer, "q") == 0)
	{
		displayLoanMenu(listLoans, listBooks);
	}
	else
	{
		printf("\n========================================================================================================================\n\n");
		printf("   MENU MODIFICATION EMPRUNT\n");
		printf("     > 1.  Modifier la date d\'emprunt\n");
		printf("     > 2.  Modifier la date de retour\n");
		printf("     > 0.  Retour au menu gestion des emprunts\n\n");
		printf("========================================================================================================================\n");


		numLoan = atoi(answer);

		do
		{
			printf("Choix: ");
			do
			{
				fgets(choice, 2, stdin);
				removeChar(choice, '\n');
			} while (strlen(choice) == 0);
		} while ((strcmp(choice, "1") != 0) && (strcmp(choice, "2") != 0) && (strcmp(choice, "0") != 0));


		ListLoans currentLoan = findLoanByNum(listLoans, numLoan);

		if (strcmp(choice, "1") == 0)
		{

			printf("Ancienne date d\'emprunt: %s\n", currentLoan->loan.dateLoan);
			do
			{
				printf("Nouvelle date d\'emprunt: ");
				do
				{
					fgets(newLoanDate, 11, stdin);
					removeChar(newLoanDate, '\n');
				} while (strlen(newLoanDate) == 0 && isValidDate(newLoanDate) != 1);

				valid = isValidDate(newLoanDate);

			} while (strlen(newLoanDate) != 10 || valid != 1);
			strcpy(editedLoan.dateLoan, newLoanDate);

			strcpy(editedLoan.dateReturn, currentLoan->loan.dateReturn);
			strcpy(editedLoan.ISBN, currentLoan->loan.ISBN);
			strcpy(editedLoan.NCE, currentLoan->loan.NCE);
			editedLoan.numLoan = currentLoan->loan.numLoan;

			listLoans = editLoan(listLoans, numLoan, editedLoan);

			returnToLoanMenu(listLoans, listBooks);
		}

		if (strcmp(choice, "2") == 0)
		{
			printf("Ancienne date de retour: %s\n", currentLoan->loan.dateReturn);
			do
			{
				printf("Nouvelle date de retour: ");
				do
				{
					fgets(newReturnDate, 11, stdin);
					removeChar(newReturnDate, '\n');
				} while (strlen(newReturnDate) == 0 && isValidDate(newReturnDate) != 1);

				valid = isValidDate(newReturnDate);

			} while (strlen(newReturnDate) != 10 || valid != 1);
			strcpy(editedLoan.dateReturn, newReturnDate);

			strcpy(editedLoan.dateLoan, currentLoan->loan.dateLoan);
			strcpy(editedLoan.ISBN, currentLoan->loan.ISBN);
			strcpy(editedLoan.NCE, currentLoan->loan.NCE);
			editedLoan.numLoan = currentLoan->loan.numLoan;

			editLoan(listLoans, numLoan, editedLoan);

			returnToLoanMenu(listLoans, listBooks);
		}

		if (strcmp(choice, "0") == 0)
		{
			displayLoanMenu(listLoans, listBooks);
		}
	}
}


ListLoans editLoan(ListLoans listLoans, int numLoan, Loan editedLoan)
{
	ElemLoan *lTmp = listLoans;
	while (lTmp != NULL)
	{
		if (lTmp->loan.numLoan == numLoan)
		{
			lTmp->loan = editedLoan;

			printf("\nEmprunt modifie avec succes !\n");
			return listLoans;
		}
		lTmp = lTmp->next; 
	}

	printf("L\'emprunt a modifier n\'existe pas !\n");
	return NULL;
}



void deleteLoanMenu(ListLoans listLoans, ListBooks listBooks)
{
	char answer[3];
	int numLoan;
	char confirm[2];

	do
	{
		printf("\nNumero de l\'emprunt a supprimer (q pour quitter): ");
		do
		{
			fgets(answer, 3, stdin);
			removeChar(answer, '\n');
		} while (strlen(answer) == 0);
	} while ((strcmp(answer, "q") != 0) && (findLoanByNum(listLoans, atoi(answer)) == NULL));

	if (strcmp(answer, "q") == 0)
	{
		displayLoanMenu(listLoans, listBooks);
	}
	else
	{
		numLoan = atoi(answer);
		do
		{
			printf("\nEtes-vous sur de vouloir supprimer cet emprunt ? (o/n)  ");

			do
			{
				fgets(confirm, 2, stdin);
				removeChar(confirm, '\n');
			} while (strlen(confirm) == 0);


		} while (strcmp(confirm, "o") != 0 && strcmp(confirm, "n") != 0);

		if (strcmp(confirm, "o") == 0)
		{
			listLoans = deleteLoanFromList(listLoans, numLoan, listBooks);
			returnToLoanMenu(listLoans, listBooks);
		}
		else
		{
			printf("\nSuppression annulee !\n");
			returnToLoanMenu(listLoans, listBooks);
		}
	}
}

void studentLoansMenu(ListLoans listLoans, ListBooks listBooks)
{
	char nce[9];

	ListLoans studentLoans = NULL;

	do
	{
		printf("\nNCE (q pour quitter): ");
		do
		{
			fgets(nce, 9, stdin);
			removeChar(nce, '\n');
		} while (strlen(nce) == 0);
	} while ((strcmp(nce, "q") != 0) && (strlen(nce) != 8));

	if (strcmp(nce, "q") == 0)
	{
		displayLoanMenu(listLoans, listBooks);
	}
	else
	{
		studentLoans = findLoansByNCE(listLoans, nce);
		if (studentLoans == NULL)
		{
			printf("\nCet etudiant n\'a emprunte aucun livre !\n\n");
			returnToLoanMenu(listLoans, listBooks);
		}
		else
		{
			ElemLoan *lTmp = studentLoans;
			while (lTmp != NULL)
			{
				printf("\n=============== Informations Emprunt ===============\n\n");
				printf("\nNumero de l\'emprunt: %d\n", lTmp->loan.numLoan);
				printf("\nISBN du livre: %s\n", lTmp->loan.ISBN);
				printf("\nDate de l\'emprunt: %s\n", lTmp->loan.dateLoan);
				printf("\nDate de retour du livre: %s\n", lTmp->loan.dateReturn);

				printf("\n");
				lTmp = lTmp->next;
			}

			returnToLoanMenu(listLoans, listBooks);
		}
	}
}




ListLoans deleteLoanFromList(ListLoans listLoans, int numLoan, ListBooks listBooks)
{
	ElemLoan *lTmp = listLoans;
	ElemLoan *lPred = listLoans;
	ElemLoan *lTmp2 = listLoans;


	if (listLoans == NULL)
	{
		printf("Aucun emprunt trouve !\n");
		return NULL;
	}

	//The element to delete is the first element of the list
	if (listLoans->loan.numLoan == numLoan)
	{
		lTmp = listLoans;
		listLoans = lTmp->next;


		incrementNbBooks(listBooks, lTmp->loan.ISBN);
		free(lTmp);


		printf("\nEmprunt supprime avec succes !\n\n");
		return listLoans;
	}
	else
	{
		lTmp = listLoans->next;
		while (lTmp != NULL)
		{
			if (lTmp->loan.numLoan == numLoan)
			{
				lTmp2 = lTmp->next;
				lPred->next = lTmp2;
				free(lTmp);

				incrementNbBooks(listBooks, listLoans->loan.ISBN);

				printf("\nEmprunt supprime avec succes !\n\n");
				return listLoans;
			}
			lTmp = lTmp->next;
			lPred = lPred->next;
		}
	}

	printf("Aucun emprunt ne correspond au numero saisi !\n");
	return listLoans;
}





void displayLoansList(ListLoans list)
{
	if (list == NULL)
	{
		printf("\nAucun emprunt a afficher !\n\n");
	}
	else
	{
		printf("\nLes emprunts enregistres sont: \n\n");
		ElemLoan *lTmp = list;
		while (lTmp != NULL)
		{
			printf("Numero d\'emprunt: %d\n", lTmp->loan.numLoan);
			printf("ISBN du livre: %s\n", lTmp->loan.ISBN);
			printf("NCE: %s\n", lTmp->loan.NCE);
			printf("Date d\'emprunt: %s\n", lTmp->loan.dateLoan);
			printf("Date de retour du livre: %s\n", lTmp->loan.dateReturn);

			printf("\n");

			lTmp = lTmp->next;
		}
	}
}


void displayLoanMenu(ListLoans listLoans, ListBooks listBooks)
{
	char choice[2];

	printf("\n\n");
	printf("                                      ===========================================\n");
	printf("                                      ====                                   ====\n");
	printf("                                      ====                                   ====\n");
	printf("                                      ====            EPI - BIBLIO           ====\n");
	printf("                                      ====        GESTION DES EMPRUNTS       ====\n");
	printf("                                      ====                                   ====\n");
	printf("                                      ===========================================\n\n");

	printf("========================================================================================================================\n\n");
	printf("   MENU GESTION DES EMPRUNTS\n");
	printf("     > 1.  Ajouter un emprunt\n");
	printf("     > 2.  Modifier un emprunt\n");
	printf("     > 3.  Supprimer un emprunt\n");
	printf("     > 4.  Afficher les emprunts d\'un etudiant\n");
	printf("     > 5.  Afficher tous les emprunts\n");
	printf("     > 0.  Retour au menu principal\n");
	printf("\n");
	printf("========================================================================================================================\n\n");

	do
	{
		printf("Choix : ");
		do
		{
			fgets(choice, 2, stdin);
			removeChar(choice, '\n');
		} while (strlen(choice) == 0);
	} while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0 && strcmp(choice, "0") != 0);

	if (strcmp(choice, "1") == 0)
	{
		Loan newLoan = createLoan(listLoans, listBooks);
		listLoans = addNewLoanToList(listLoans, listBooks, newLoan, 0);

		returnToLoanMenu(listLoans, listBooks);
	}

	if (strcmp(choice, "2") == 0)
	{
		editLoanMenu(listLoans, listBooks);

		returnToLoanMenu(listLoans, listBooks);
	}

	if (strcmp(choice, "3") == 0)
	{
		deleteLoanMenu(listLoans, listBooks);

		returnToLoanMenu(listLoans, listBooks);
	}

	if (strcmp(choice, "4") == 0)
	{
		studentLoansMenu(listLoans, listBooks);

		returnToLoanMenu(listLoans, listBooks);
	}

	if (strcmp(choice, "5") == 0)
	{
		displayLoansList(listLoans);

		returnToLoanMenu(listLoans, listBooks);
	}
	if (strcmp(choice, "0") == 0)
	{
		displayMainMenu(listBooks, listLoans);
	}

}

void returnToLoanMenu(ListLoans listLoans, ListBooks listBooks)
{
	char retLoanMenu[2];

	do
	{
		printf("Appuyer sur q pour retourner au menu de gestion des emprunts.  ");
		do
		{
			fgets(retLoanMenu, 2, stdin);
			removeChar(retLoanMenu, '\n');
		} while (strlen(retLoanMenu) == 0);
	} while (strcmp(retLoanMenu, "q") != 0);


	displayLoanMenu(listLoans, listBooks);
}


int getListLength(ListLoans listLoans)
{
	int total = 0;
	ElemLoan *lTmp = listLoans;
	if (listLoans == NULL)
	{
		return 0;
	}
	else
	{
		while (lTmp != NULL)
		{
			total++;

			lTmp = lTmp->next;
		}
	}
	return total;
}

ListLoans initListLoansFromFile(FILE* file, ListLoans listLoans, ListBooks listBooks)
{
	char line[1000];
	Loan newLoan;
	int i = 0;

	char isbn[5] = "";
	char nce[9] = "";
	char dateLoan[11] = "";
	char dateReturn[11] = "";
	char numLoanString[4] = "";
	int numLoan;


	file = fopen("./Files/Loans.txt", "r");
	if (file == NULL)
	{
		printf("\nLe fichier n\'existe pas !\n");
	}
	else
	{
		fseek(file, 180, SEEK_SET);
		while (fgets(line, 1000, file) != NULL)
		{
			for (i = 0; i < 4; i++)
			{
				if (line[i] == ' ' && line[i + 1] == ' ')
				{
					break;
				}

				concatChar(numLoanString, line[i]);
			}
			numLoan = atoi(numLoanString);
			newLoan.numLoan = numLoan;
			strcpy(numLoanString, "");

			for (i = 13; i < 21; i++)
			{
				concatChar(nce, line[i]);
			}


			strcpy(newLoan.NCE, nce);
			strcpy(nce, "");

			for (i = 29; i < 33; i++)
			{
				concatChar(isbn, line[i]);
			}


			strcpy(newLoan.ISBN, isbn);
			strcpy(isbn, "");


			for (i = 53; i < 63; i++)
			{
				concatChar(dateLoan, line[i]);
			}
			
			strcpy(newLoan.dateLoan, dateLoan);
			strcpy(dateLoan, "");


			for (i = 77; i < 87; i++)
			{
				concatChar(dateReturn, line[i]);
			}

			strcpy(newLoan.dateReturn, dateReturn);
			strcpy(dateReturn, "");

			listLoans = addNewLoanToList(listLoans, listBooks, newLoan, 1);

		}

		fclose(file);
	}

	return listLoans;
}


void insertListLoansIntoFile(ListLoans listLoans)
{
	int nbSpaces = 0;
	int i = 0; 

	FILE *file = fopen("./Files/Loans.txt", "w");
	if (file == NULL)
	{
		printf("\nErreur lors de l'ouverture du fichier !\n");
	}
	else
	{
		fprintf(file, "NUM EMP      NCE             ISBN                    DATE EMPRUNT            DATE RETOUR\n");
		fprintf(file, "----------------------------------------------------------------------------------------\n");

		ElemLoan *lTmp = listLoans;
		while (lTmp != NULL)
		{
			nbSpaces = getNbSpacesForNumLoan(lTmp->loan.numLoan);
			fprintf(file, "%d", lTmp->loan.numLoan);
			for (i = 0; i < nbSpaces; i++)
			{
				fprintf(file, " ");
			}

			fprintf(file, "%s        ", lTmp->loan.NCE);

			fprintf(file, "%s                    ", lTmp->loan.ISBN);
			
			fprintf(file, "%s              ", lTmp->loan.dateLoan);

			fprintf(file, "%s\n", lTmp->loan.dateReturn);

			lTmp = lTmp->next;
		}
	}
}

int getNbSpacesForNumLoan(int num)
{
	if (num > 0 && num < 10)
	{
		return 12;
	}
	
	if (num >= 10 && num < 100)
	{
		return 11;
	}

	if (num >= 100 && num < 1000)
	{
		return 10;
	}

	return 10;
}

ListLoans addNewStudentLoanToList(ListLoans studentLoans, Loan loan)
{
	ElemLoan *newLoan = malloc(sizeof(ElemLoan));
	if (newLoan == NULL)
	{
		printf("Erreur Memoire !");
		exit(0);
	}

	newLoan->loan = loan;
	newLoan->next = NULL;

	if (studentLoans == NULL)
	{
		return newLoan;
	}
	else
	{
		ElemLoan *lTmp = studentLoans;
		while (lTmp->next != NULL)
		{
			lTmp = lTmp->next;
		}
		lTmp->next = newLoan;
	}

	return studentLoans;
}


ListLoans findLoansByNCE(ListLoans listLoans, char *nce)
{
	ListLoans studentLoans = NULL;
	ElemLoan *lTmp = listLoans;

	if (listLoans == NULL)
	{
		return NULL;
	}

	while (lTmp != NULL)
	{
		if (strcmp(lTmp->loan.NCE, nce) == 0)
		{
			studentLoans = addNewStudentLoanToList(studentLoans, lTmp->loan);
		}

		lTmp = lTmp->next;
	}

	return studentLoans;
}


int isValidDate(char *date)
{
	int i;

	int day;
	int month;
	int year;

	char dayString[3] = "";
	char monthString[3] = "";
	char yearString[5] = "";

	for (i = 0; i < 2; i++)
	{
		concatChar(dayString, date[i]);
	}
	day = atoi(dayString);
	if (day < 1 || day > 31)
	{
		return 0;
	}

	for (i = 3; i < 5; i++)
	{
		concatChar(monthString, date[i]);
	}
	month = atoi(monthString);
	if (month < 1 || month > 12)
	{
		return 0;
	}

	for (i = 6; i < 10; i++)
	{
		concatChar(yearString, date[i]);
	}
	year = atoi(yearString);
	if (year != 2018 && year != 2019)
	{
		return 0;
	}

	return 1;
}


/****************************************************************************************/




void main()
{
	ListBooks listBooks = NULL;
	ListLoans listLoans = NULL;
	FILE *fileBooks = NULL;
	FILE *fileLoans = NULL;

	listBooks = initListBooksFromFile(fileBooks, listBooks);

	listLoans = initListLoansFromFile(fileLoans, listLoans, listBooks);

	
    displayMainMenu(listBooks, listLoans);

}