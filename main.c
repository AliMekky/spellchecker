#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    char* word;
    struct Node* left;
    struct Node* right;
} Node;

Node* newNode(char* word) //Constructor
{
    Node* n = malloc(sizeof(Node));
    n->word = malloc(strlen(word)+1);
    strcpy(n->word,word);
    n->left = NULL;
    n->right = NULL;
    return n;
}
Node* insert(Node* root,char* word)  //or void insert(Node** root,int x);
{
    if(root == NULL) // tree is empty, make this element the root
        root = newNode(word);
    else if(strcasecmp(word,root->word) > 0) // add it to the right branch
        root->right = insert(root->right,word);
    else if(strcasecmp(word,root->word) < 0)  // add it to the left branch
        root->left = insert(root->left,word);
    return root;
}
int countNodes(Node* root) // function to count number of nodes
{
    if(root == NULL)
        return 0;
    return 1 + countNodes(root->right) + countNodes(root->left);
}
int max(int x,int y) // function to return the max between two numbers
{
    return x>y?x:y;
}

int height(Node* root) // function to return the height of the tree
{
    if(!root)
        return -1;
    else
        return 1+max(height(root->left),height(root->right));
}
Node* readFile(char* filename)
{
    FILE* dic = fopen(filename,"r");
    char word[100];
    Node* root = NULL;
    while(!feof(dic))  // read words from dictionary and insert them in binary search tree.
    {
        fscanf(dic,"%s",word);
        root=insert(root,word);
    }
    fclose(dic);

    return root;
}
Node* findMin(Node* root)
{
    if(root == NULL) // no elements in the tree
        return NULL;
    else if(root->left == NULL) //min is the last element on the left
        return root;
    else
        return findMin(root->left); // keep moving to the left
}

Node* findMax(Node* root)
{
    if(root == NULL) // no elements in the tree
        return NULL;
    else if(root->right == NULL) //max is the last element on the right
        return root;
    else
        return findMax(root->right); // keep moving to the right
}

void pPostOrder(Node* root) // left right root(just to check we loaded the dictionary)
{
    if(root)
    {
        pPostOrder(root->left);
        pPostOrder(root->right);
        printf("%s\n",root->word);
    }
}


void SuccPred(Node* root,char* key)
{
    Node* temp = root; // change temp instead of root
    Node* succ = NULL;
    Node* pred = NULL;
    Node* last;


    if(root == NULL)
        return;

    while(temp)
    {
        if((strcasecmp(key,temp->word) > 0 && temp->right == NULL) || (strcasecmp(key,temp->word) < 0 && temp->left == NULL))
        {
            //( if key is bigger than word->temp and temp has no right child )or( key is smaller than word->temp and temp has no left child )
            // then temp is the last node.
            printf("%s ",temp->word);
            last = temp;
            temp = root;  // returning temp to the root to start new loop.
            break;

        }
        else if((strcasecmp(key,temp->word) > 0) &&temp->right != NULL)
        {
            temp = temp->right; // updating temp to search the right branch

        }
        else if((strcasecmp(key,temp->word) < 0 && temp->left != NULL))
            temp = temp->left; // updating temp to search the left branch
    }

    if(last->right != NULL)
    {
        succ = findMin(last->right); // if last node has right branch then the successor is the minimum node in that branch
    }
    else
    {
        while(strcasecmp(temp->word,last->word) != 0)  // loop until you reach the last node.
        {
            if(strcasecmp(temp->word,last->word) > 0)  // if temp->word is bigger than last->word make it the successor and update temp to the left;
            {
                succ = temp;
                temp = temp->left;
            }
            else
            {
                temp = temp->right; // update temp to the right
            }
        }
    }
    temp = root; // returning temp to the root to start new loop
    if(succ != NULL)
        printf("%s ",succ->word); // printing successor word

    if(last->left != NULL)
    {
        pred = findMax(last->left); // if the last node has left branch then the predecessor is the maximum in that branch
    }
    else
    {
        while(strcasecmp(temp->word,last->word) != 0)  // loop until you reach last node
        {
            if(strcasecmp(temp->word,last->word) < 0)
            {
                pred = temp; // temp->word is smaller than last->word make it the predecessor and update temp to the right
                temp = temp->right;
            }
            else
            {
                temp = temp->left; // update temp to the left;
            }
        }
    }

    if(pred != NULL)
        printf("%s ",pred->word); // printing predecessor

}



void searchIter(Node* root,char* key)
{

    Node* temp = root; // change temp instead of root
    while(temp)     // keep searching until temp is NULL
    {
        if(strcasecmp(temp->word,key) == 0 && strlen(key) == strlen(temp->word))  // found the key, print the word is correct,
        {
            printf("%s : Correct\n",key);

            return;
        }

        else if(strcasecmp(key,temp->word) > 0) // search the right branch only
            temp = temp->right;
        else
            temp = temp->left;    // search the left branch only
    }
    if(!temp)  // if the word isnot found and temp = null get last,successor and predecessor nodes
    {
        printf("%s : incorrect ,Suggestions: ",key);
        SuccPred(root,key);
        printf("\n");
    }
}






int main()
{
    FILE* f = fopen("EN-US-Dictionary.txt","r");
    char sentence[100];
    if(f)
    {
        printf("Dictionary loaded successfully!!!");
        printf("==============================\n");
        Node* root = readFile("EN-US-Dictionary.txt");

        printf("Height = %d\n",height(root));
        printf("==============================\n");
        printf("Size = %d\n",countNodes(root));
        printf("==============================\n");
        printf("Enter a sentence: \n");
        scanf("%[^\n]",sentence);

        char* token = strtok(sentence," ");

        while(token)
        {
            searchIter(root,token); // tokenizing input sentence and check each word.
            token = strtok(NULL," ");
        }

        fclose(f);

    }
    else
        printf("FILE NOT OPEN");
    fclose(f);

    return 0;
}



// Ali Hassan Mekky  6850
// Mohamed Alaa 6886
// Seif el-Tahtawy 6825
