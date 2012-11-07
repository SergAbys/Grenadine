/*
 *  papa.h
 *
 *  nicolas.danet@free.fr
 *
 */

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include "ext.h"                            
#include "ext_obex.h"
#include "jpatcher_api.h"
#include "jgraphics.h"
#include "ext_common.h"     
#include "ext_systhread.h"
#include "commonsyms.h"

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#include <math.h>   
#include <time.h>
#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------    
// -------------------------------------------------------------------------------------------------------------

#define DEFAULT_OUTPUT_LAYER_SIZE       4

#define MAXIMUM_OUTPUT_LAYER_SIZE       128
#define MAXIMUM_SYNAPSES_PER_NODE       128
#define MAXIMUM_NODES_PER_LAYER         128

#define MAXIMUM_NODES                   512
#define MAXIMUM_SYNAPSES                2048 

#define QUEUE_SIZE                      512

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

#define WEIGHT_FEEDBACK                 1.
#define WEIGHT_WRONG                    0.

#define STATE_WHITE                     0
#define STATE_GREY                      1
#define STATE_BLACK                     2
#define STATE_NO_BACKPROPAGATION       -1

#define DISTANCE_OUTPUT_LAYER          -1
#define DISTANCE_LONELY_NODE           -2

#define SYNAPSE                         0
#define SYNAPSE_FEEDBACK                1
#define SYNAPSE_WRONG                  -1

#define MODE_NORMAL                     1
#define MODE_NO_BACKWARD                0
#define MODE_FINAL                     -1
#define MODE_START                     -2

#define TRANSFER_ERROR                 -1
#define TRANSFER_LINEAR                 0
#define TRANSFER_SIGMOID                1
#define TRANSFER_TANH                   2

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _node {
    long        mode;
    long        state;
    long        distance;
    long        transfer;
    long        index;
    long        type        [MAXIMUM_SYNAPSES_PER_NODE];
    long        destination [MAXIMUM_SYNAPSES_PER_NODE];
    double      weight      [MAXIMUM_SYNAPSES_PER_NODE];
    double      potential;
    double      delayA;
    double      delayB;
    double      signal;
    double      error;
    double      tempPotential;
    double      tempSignal;
    t_object    *object;
    t_object    *box;
    t_object    *line       [MAXIMUM_SYNAPSES_PER_NODE];
    } t_node;
    
// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

typedef struct _papa {
    t_object            ob;
    t_object            *me;
    t_object            *oscar;
    t_object            *meBox;
    t_object            *oscarBox;
    t_node              *stock;
    long                *graph;
    long                *graphMap;
    long                stockSize;
    long                headQueue;
    long                tailQueue;
    long                valueQueue;
    long                sizeInputLayer;
    long                sizeOutputLayer;
    long                sizeHiddenLayers;
    long                maximumDistance;
    long                saveDataWithPatcher;
    long                papaTransfer;
    long                oscarTransfer;
    long                lockDump;
    long                error;
    long                *queue;
    double              bias;
    double              threshold;
    double              learning;
    double              totalError;
    double              *vectorOut;
    double              *vectorIn;
    double              *vectorTarget;
    t_atomarray         *headData;
    t_atomarray         *graphData;
    t_atomarray         *synapsesData;
    t_jrgba             lineSynapse;
    t_jrgba             lineFeedback;
    t_jrgba             lineWrong;
    t_jrgba             boxLinear;
    t_jrgba             boxSigmoid;
    t_jrgba             boxTanh;
    t_jrgba             boxError;
    t_systhread_mutex   mutex;
    void                *clock;
    void                *outlet[MAXIMUM_OUTPUT_LAYER_SIZE];
    } t_papa;
    
// -------------------------------------------------------------------------------------------------------------    
// -------------------------------------------------------------------------------------------------------------

void    *papa_new                       (t_symbol *s, long argc, t_atom *argv);
void    papa_free                       (t_papa *x);
void    papa_assist                     (t_papa *x, void *b, long m, long a, char *s);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void    papa_dblclick                   (t_papa *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void    papa_clear                      (t_papa *x);
void    papa_anything                   (t_papa *x, t_symbol *s, long argc, t_atom *argv);
void    papa_int                        (t_papa *x, long n);
void    papa_float                      (t_papa *x, double f);
void    papa_list                       (t_papa *x, t_symbol *s, long argc, t_atom *argv);
void    papa_learn                      (t_papa *x, t_symbol *s, long argc, t_atom *argv);
void    papa_target                     (t_papa *x, t_symbol *s, long argc, t_atom *argv);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void    papa_dump                       (t_papa *x);
void    papa_task                       (t_papa *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void    papa_findOscar                  (t_papa *x);
long    papa_iterateToFindOscar         (t_papa *x, t_object *box);
void    papa_rebuildFromData            (t_papa *x);
void    papa_appendToDictionary         (t_papa *x, t_dictionary *bd);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void    papa_propagation                (t_papa *x);
double  papa_computeErrors              (t_papa *x);
void    papa_backpropagation            (t_papa *x);
void    papa_initialisation             (t_papa *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

void    papa_buildGraph                 (t_papa *x);
long    papa_iterateToBuildGraph        (t_papa *x, t_object *box);
long    papa_stockGetIndexByPtrOutlet   (t_papa *x, t_object *object, long n);
long    papa_stockGetIndexByPtrInlet    (t_papa *x, t_object *object, long n);
void    papa_graphBoxColor              (t_papa *x, t_object *box, long n);
void    papa_freeGraph                  (t_papa *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------

double  papa_randomGaussian             (void);
long    papa_putFifo                    (t_papa *x, long n);
long    papa_getFifo                    (t_papa *x);

// -------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------