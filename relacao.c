#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int xIndex;
    int yIndex;
} OrderedPair;

typedef struct
{
    char name[20];
    bool value;
    OrderedPair *orderedPairs;
    int numberOfOrderedPairs;
} Property;

typedef struct
{
    int numberOfElements;
    int *elements;
    bool **relations;
    bool isEquivalent;
    bool isPartialOrder;
    Property reflexive;
    Property irreflexive;
    Property symmetric;
    Property antisymmetric;
    Property asymmetric;
    Property transitive;
    Property transitiveClosure;
} Set;

void allocateElementsAndRelations(Set *set)
{
    set->elements = malloc(set->numberOfElements);

    set->relations = malloc(set->numberOfElements * sizeof(bool *));

    for (int i = 0; i < set->numberOfElements; i++)
        set->relations[i] = malloc(set->numberOfElements);
}

void getElements(FILE *fp, int *elements)
{
    char line[200], *val = NULL;
    int numberOfNumbs = 0, valsRead, value;
    if (fgets(line, 200, fp) == NULL)
    { /* EOF */
        fprintf(stderr, "No input\n");
        return;
    }
    val = strtok(line, " ");
    valsRead = sscanf(val, "%d", &value);

    while (valsRead > 0)
    {
        elements[numberOfNumbs] = value;

        numberOfNumbs++;
        val = strtok(NULL, " ");
        valsRead = (val == NULL) ? 0 : sscanf(val, "%d", &value);
    }
}

int findElementIndex(Set *set, int element)
{
    for (int i = 0; i < set->numberOfElements; i++)
    {
        if (set->elements[i] == element)
            return i;
    }
    return -1;
}

void getRelations(FILE *fp, Set *set)
{
    if (feof(fp))
    {
        printf("Nenhuma relação definida");
        return;
    }
    int x = 0, y = 0, xIndex = 0, yIndex = 0;

    do
    {
        //printRelations();
        fscanf(fp, "%d", &x);
        fscanf(fp, "%d", &y);
        xIndex = findElementIndex(set, x);
        yIndex = findElementIndex(set, y);

        if (xIndex == -1)
        {
            printf("Elemento nao encontrado %d", x);
            break;
        }

        if (yIndex == -1)
        {
            printf("Elemento nao encontrado %d", y);
            break;
        }

        set->relations[xIndex][yIndex] = true;
    } while (!feof(fp));
}

void deallocateElementsAndRelations(Set *set)
{
    free(set->elements);

    for (int i = 0; i < set->numberOfElements; i++)
        free(set->relations[i]);

    free(set->relations);
}

void printElements(Set set)
{
    printf("\nElementos: \n");
    for (int i = 0; i < set.numberOfElements; i++)
        printf("%d\n", set.elements[i]);
}

void printRelations(Set set)
{
    printf("\nRelacoes: \n");
    for (int i = 0; i < set.numberOfElements; i++)
    {
        for (int j = 0; j < set.numberOfElements; j++)
            printf("%3d ", set.relations[i][j]);

        printf("\n");
    }
}

void allocateOrderedPairArrays(Set *set)
{
    size_t dynamicArraySize = set->numberOfElements * set->numberOfElements * sizeof(OrderedPair);
    set->reflexive.orderedPairs = (OrderedPair *)malloc(dynamicArraySize);
    set->irreflexive.orderedPairs = (OrderedPair *)malloc(dynamicArraySize);
    set->symmetric.orderedPairs = (OrderedPair *)malloc(dynamicArraySize);
    set->antisymmetric.orderedPairs = (OrderedPair *)malloc(dynamicArraySize);
    set->asymmetric.orderedPairs = (OrderedPair *)malloc(dynamicArraySize);
    set->transitive.orderedPairs = (OrderedPair *)malloc(dynamicArraySize);
    set->transitiveClosure.orderedPairs = (OrderedPair *)malloc(dynamicArraySize);
}

void deallocateOrderedPairArrays(Set *set)
{
    free(set->reflexive.orderedPairs);
    free(set->irreflexive.orderedPairs);
    free(set->symmetric.orderedPairs);
    free(set->antisymmetric.orderedPairs);
    free(set->asymmetric.orderedPairs);
    free(set->transitive.orderedPairs);
}

void addToOrderedPairArray(Property *property, OrderedPair newOrderedPair)
{
    for(int i = 0; i < property->numberOfOrderedPairs; i++) {
        if(property->orderedPairs[i].xIndex == newOrderedPair.xIndex && property->orderedPairs[i].yIndex == newOrderedPair.yIndex)
            return;
    }
    int currentIndex = property->numberOfOrderedPairs;
    property->orderedPairs[currentIndex] = newOrderedPair;
    property->numberOfOrderedPairs++;
}

void isSetReflexive(Set *set)
{
    set->reflexive.value = true;
    for (int x = 0; x < set->numberOfElements; x++)
    {
        for (int y = 0; y < set->numberOfElements; y++)
        {
            if (x == y && !set->relations[x][y])
            {
                OrderedPair orderedPair = {x, y};
                addToOrderedPairArray(&set->reflexive, orderedPair);
                set->reflexive.value = false;
            }
        }
    }
}

void isSetIrreflexive(Set *set)
{
    set->irreflexive.value = true;
    for (int x = 0; x < set->numberOfElements; x++)
    {
        for (int y = 0; y < set->numberOfElements; y++)
        {
            if (x == y && set->relations[x][y])
            {
                OrderedPair orderedPair = {x, y};
                addToOrderedPairArray(&set->irreflexive, orderedPair);
                set->irreflexive.value = false;
            }
        }
    }
}

void isSetSymmetric(Set *set)
{
    set->symmetric.value = true;
    for (int x = 0; x < set->numberOfElements; x++)
    {
        for (int y = 0; y < set->numberOfElements; y++)
        {
            if (set->relations[x][y] && !set->relations[y][x])
            {
                OrderedPair orderedPair = {y, x};
                addToOrderedPairArray(&set->symmetric, orderedPair);
                set->symmetric.value = false;
            }
        }
    }
}

void isSetAntisymmetric(Set *set)
{
    set->antisymmetric.value = true;
    for (int x = 0; x < set->numberOfElements; x++)
    {
        for (int y = 0; y < set->numberOfElements; y++)
        {
            if (set->relations[x][y] &&  set->relations[y][x] && x != y)
            {
                OrderedPair orderedPair = {x, y};
                addToOrderedPairArray(&set->antisymmetric, orderedPair);
                set->antisymmetric.value = false;
            }
        }
    }
}

void isSetAsymmetric(Set *set)
{
    set->asymmetric.value = true;
    for (int x = 0; x < set->numberOfElements; x++)
    {
        for (int y = 0; y < set->numberOfElements; y++)
        {
            if (set->relations[x][y] && set->relations[y][x])
            {
                OrderedPair orderedPair = {x, y};
                set->asymmetric.value = false;
            }
        }
    }
}

void isSetTransitive(Set *set)
{
    set->transitive.value = true;
    for (int x = 0; x < set->numberOfElements; x++)
    {
        for (int y = 0; y < set->numberOfElements; y++)
        {
            for (int z = 0; z < set->numberOfElements; z++)
            {
                if (set->relations[x][y] && set->relations[y][z] && !set->relations[x][z])
                {
                    OrderedPair orderedPair = {x, z};
                    addToOrderedPairArray(&set->transitive, orderedPair);
                    set->transitive.value = false;
                }
            }
        }
    }
}

void isSetEquivalent(Set *set)
{
    set->isEquivalent = set->reflexive.value && set->symmetric.value && set->transitive.value;
}

void isSetPartialOrder(Set *set)
{
    set->isPartialOrder = set->reflexive.value && set->antisymmetric.value && set->transitive.value;
}

void getTransitiveClosure(Set *set)
{
    for(int x = 0; x < set->numberOfElements; x++) {
        for(int y = 0; y < set->numberOfElements; y++) {
            if(set->relations[x][y]) {
                OrderedPair orderedPair = {x, y};
                addToOrderedPairArray(&set->transitiveClosure, orderedPair);
            }
        }
    }

    for(int i =0; i < set->transitive.numberOfOrderedPairs; i++)
        addToOrderedPairArray(&set->transitiveClosure, set->transitive.orderedPairs[i]);
}
void printMissingRelations(Property property, int *elements)
{
    for(int i = 0; i < property.numberOfOrderedPairs; i++)
        printf("(%i,%i); ", elements[property.orderedPairs[i].xIndex], elements[property.orderedPairs[i].yIndex]);
    printf("\n");
}

void printProperty(Property property, int *elements)
{
    printf("%s: %s", property.name, property.value ? "V" : "F");

    if(property.numberOfOrderedPairs)
        printf("\n");
    printMissingRelations(property, elements);
}

void printProperties(Set set)
{
    printf("\n");
    printProperty(set.reflexive, set.elements);
    printProperty(set.irreflexive, set.elements);
    printProperty(set.symmetric, set.elements);
    printProperty(set.antisymmetric, set.elements);
    printProperty(set.asymmetric, set.elements);
    printProperty(set.transitive, set.elements);
    printf("Relacao de equivalencia: %s\n", set.isEquivalent ? "V" : "F");
    printf("Relacao de ordem parcial: %s\n", set.isPartialOrder ? "V" : "F");
    printf("Fecho transitivo da relacao: ");
    printMissingRelations(set.transitiveClosure, set.elements);
}

void printPair(Set set, OrderedPair orderedPair)
{
    printf("(%d,%d); ", set.elements[orderedPair.xIndex], set.elements[orderedPair.yIndex]);
}

void printPairs(Set set, Property property)
{
    printf("\n");
    for (int i = 0; i < property.numberOfOrderedPairs; i++)
        printPair(set, property.orderedPairs[i]);
}

void getProperties(Set *set)
{
    isSetReflexive(set);
    isSetIrreflexive(set);
    isSetSymmetric(set);
    isSetAntisymmetric(set);
    isSetAsymmetric(set);
    isSetTransitive(set);
    isSetEquivalent(set);
    isSetPartialOrder(set);
    getTransitiveClosure(set);
}

int main()
{
    FILE *fp;

    Set set = {
        0,
        NULL,
        NULL,
        false,
        false,
        {"Reflexiva", true},
        {"Irreflexiva", true},
        {"Simetrica", true},
        {"Anti-simetrica", true},
        {"Assimetrica", true},
        {"Transitiva", true}};

    fp = fopen("./entrada.txt", "r");

    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return 0;
    }

    fscanf(fp, "%d", &set.numberOfElements);
    allocateElementsAndRelations(&set);

    getElements(fp, set.elements);
    getRelations(fp, &set);

    fclose(fp);

    allocateOrderedPairArrays(&set);
    getProperties(&set);

    printProperties(set);

    deallocateOrderedPairArrays(&set);
    deallocateElementsAndRelations(&set);
    return 0;
}