#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      
#include <sys/types.h>  
#include <sys/wait.h>  
#include <string.h>
#include "knn.h"

/*****************************************************************************/
/* Do not add anything outside the main function here. Any core logic other  */
/*   than what is described below should go in `knn.c`. You've been warned!  */
/*****************************************************************************/

/**
 * main() takes in the following command line arguments.
 *   -K <num>:  K value for kNN (default is 1)
 *   -d <distance metric>: a string for the distance function to use
 *          euclidean or cosine (or initial substring such as "eucl", or "cos")
 *   -p <num_procs>: The number of processes to use to test images
 *   -v : If this argument is provided, then print additional debugging information
 *        (You are welcome to add print statements that only print with the verbose
 *         option.  We will not be running tests with -v )
 *   training_data: A binary file containing training image / label data
 *   testing_data: A binary file containing testing image / label data
 *   (Note that the first three "option" arguments (-K <num>, -d <distance metric>,
 *   and -p <num_procs>) may appear in any order, but the two dataset files must
 *   be the last two arguments.
 * 
 * You need to do the following:
 *   - Parse the command line arguments, call `load_dataset()` appropriately.
 *   - Create the pipes to communicate to and from children
 *   - Fork and create children, close ends of pipes as needed
 *   - All child processes should call `child_handler()`, and exit after.
 *   - Parent distributes the test dataset among children by writing:
 *        (1) start_idx: The index of the image the child should start at
 *        (2)    N:      Number of images to process (starting at start_idx)
 *     Each child should get at most N = ceil(test_set_size / num_procs) images
 *      (The last child might get fewer if the numbers don't divide perfectly)
 *   - Parent waits for children to exit, reads results through pipes and keeps
 *      the total sum.
 *   - Print out (only) one integer to stdout representing the number of test 
 *      images that were correctly classified by all children.
 *   - Free all the data allocated and exit.
 *   - Handle all relevant errors, exiting as appropriate and printing error message to stderr
 */
void usage(char *name) {
    fprintf(stderr, "Usage: %s -v -K <num> -d <distance metric> -p <num_procs> training_list testing_list\n", name);
}

int main(int argc, char *argv[]) {

    int opt;
    int K = 1;             // default value for K
    char *dist_metric = "euclidean"; // default distant metric
    int num_procs = 1;     // default number of children to create
    int verbose = 0;       // if verbose is 1, print extra debugging statements
    int total_correct = 0; // Number of correct predictions

    while((opt = getopt(argc, argv, "vK:d:p:")) != -1) {
        switch(opt) {
        case 'v':
            verbose = 1;
            break;
        case 'K':
            K = atoi(optarg);
            break;
        case 'd':
            dist_metric = optarg;
            break;
        case 'p':
            num_procs = atoi(optarg);
            break;
        default:
            usage(argv[0]);
            exit(1);
        }
    }

    if(optind >= argc) {
        fprintf(stderr, "Expecting training images file and test images file\n");
        exit(1);
    } 

    char *training_file = argv[optind];
    optind++;
    char *testing_file = argv[optind];

    // TODO The following lines are included to prevent compiler warnings
    // and should be removed when you use the variables.
    (void)K;
    (void)dist_metric;
    (void)num_procs;
  
    // Set which distance function to use
    /* You can use the following string comparison which will allow
     * prefix substrings to match:
     * 
     * If the condition below is true then the string matches
     * if (strncmp(dist_metric, "euclidean", strlen(dist_metric)) == 0){
     *      //found a match
     * }
     */ 
  
    // TODO
    double (*fptr)(Image *, Image *) = NULL;
    if (strncmp(dist_metric, "euclidean", strlen(dist_metric)) == 0){
        fptr = distance_euclidean;
    }

    if (strncmp(dist_metric, "cosine", strlen(dist_metric)) == 0){
        fptr = distance_cosine;
    }


    // Load data sets
    if(verbose) {
        fprintf(stderr,"- Loading datasets...\n");
    }
    
    Dataset *training = load_dataset(training_file);
    if ( training == NULL ) {
        fprintf(stderr, "The data set in %s could not be loaded\n", training_file);
        exit(1);
    }

    Dataset *testing = load_dataset(testing_file);
    if ( testing == NULL ) {
        fprintf(stderr, "The data set in %s could not be loaded\n", testing_file);
        exit(1);
    }

    // Create the pipes and child processes who will then call child_handler
    if(verbose) {
        printf("- Creating children ...\n");
    }

    // TODO
    int fd1[num_procs][2];
    int fd2[num_procs][2];
    int total_img = testing -> num_items;
    int num_img = total_img / num_procs;
    if(total_img % num_procs != 0){
        num_img += 1;
    }
    int start_img = 0;
    for(int i = 0; i < num_procs; i ++){
        if(pipe(fd1[i]) < 0){
            perror("pipe");
            exit(1);
        }
        if(pipe(fd2[i]) < 0){
            perror("pipe");
            exit(1);
        }

        int r = fork();
        if(r < 0){
            perror("fork");
            exit(1);
        }
        if(r == 0){
            close(fd1[i][1]);
            close(fd2[i][0]);
            /*
            for(int j = 0; j < i; j++){
                close(fd1[j][1]);
                close(fd2[j][0]);
            }
            */
            child_handler(training, testing, K, fptr,fd1[i][0], fd2[i][1]);
            close(fd1[i][0]);
            close(fd2[i][1]);
            exit(0);
        }
    // Distribute the work to the children by writing their starting index and
    // the number of test images to process to their write pipe

    // TODO
        else if(r > 0){
            write(fd1[i][1],&start_img, 1 * sizeof(int));
            start_img += num_img;
            if(write(fd1[i][1],&num_img, 1 * sizeof(int)) == 0){
                fprintf(stderr, "write");
                exit(0);
            }

            close(fd1[i][1]);
        }
    }
    // Wait for children to finish
    if(verbose) {
        printf("- Waiting for children...\n");
    }

    // TODO
    for(int h = 0; h < num_procs; h++){
        int status;
        wait(&status);
    // When the children have finised, read their results from their pipe
 
    // TODO
        if(WIFEXITED(status)){
            int n = 0;
            if(read(fd2[h][0], &n, 1 * sizeof(int)) == 0){
                fprintf(stderr, "read");
                exit(0);
            }
            close(fd2[h][0]);
            total_correct += n;
        }
    }


    if(verbose) {
        printf("Number of correct predictions: %d\n", total_correct);
    }

    // This is the only print statement that can occur outside the verbose check
    printf("%d\n", total_correct);

    // Clean up any memory, open files, or open pipes

    // TODO
    free_dataset(training);
    free_dataset(testing);
    for(int l = 0; l < num_procs; l++){
        close(fd1[l][0]);
        close(fd2[l][1]);
    }
    return 0;
}
