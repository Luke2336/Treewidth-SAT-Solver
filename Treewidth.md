# Treewidth & SAT

## Treewidth 

[Wikipedia](https://en.wikipedia.org/wiki/Treewidth)

-   A **tree decomposition** of a graph $G = (V, E)$ is a tree $T$ with nodes $X_1, \dots, X_n$, where $X_i$ is a subset of $V$ satisfying the following properties.
    1.  $\cup_i X_i = V$
    2.  $v \in X_i \cap X_j \implies$ $v \in X_k, \forall$ nodes $X_k$ on the path between $X_i$ and $X_j$
    3.  $\forall (u, v) \in E$, $\exists X_i$ s.t. $\{u, v\} \subseteq X_i$.

-   The **width** of a tree decomposition is $\max_i |X_i| - 1$.

-   The **treewidth** $tw(G)$ is the minimum width among all possible tree decompositions.
    -   $tw(T) = 1$.
    -   $tw(K_n) = n - 1$

-   For any fixed constant $k$, the graphs of treewidth at most $k$ are called the **partial $k$-trees**.

### FPT

-   Exact: $O(1.7347^n)$
-   $2k+1$-approx: $2^{O(k)}n$

## Encoding Treewidth into SAT

-   https://link.springer.com/chapter/10.1007/978-3-642-02777-2_6
-   SAT conference 2009

### Problem

Decide whether the treewidth of a given graph is at most $k$.

-   NP-complete

### Encoding

-   $O(k|V|^2)$ variables
-   $O(|V|^3)$ clauses
-   The length of each clause is bounded by $4$.

---

Linear ordering: $v_1, v_2, \dots, v_n$

-   For each pair of non-adjacent vertices $v_i$ and $v_j$, we successively add an edge to $E$ iff. $v_i$ and $v_j$ have a common predecessor. (Triangulartion)
-   The width of the tree decomposition $ = max_{v_i \in V} |\{\{v_i, v_j\} \in E : j > i\}|$

---

-   $ord_{i, j} = \begin{cases}1, i < j\ \and\ \{v_i, v_j\} \in E \\0, \text{otherwise.}\end{cases}$
-   Transitivity: $ord_{i, j} \and ord_{j, l} \to ord_{i, l}$
-   $arc_{i, j}$ is used to count the number of successor

---

1.  Add $2m$ clauses: $ord_{i, j} \to arc_{i, j}$ and $\neg ord_{i, j} \to arc_{j, i}$, $\forall i < j \and \{v_i, v_j\} \in E$.
2.  Add $n(n - 1)(n - 2)$ clauses: $arc_{i, j} \and arc_{i, l} \and ord_{j, l} \to arc_{j, l}$ and $arc_{i j} \and arc_{i, l} \and \neg ord_{j, l} \to arc_{l, j}$, $\forall i \neq j, i \neq l, j < l$
3.  Add $n$ unit clauses for neglecting self loops: $\neg arc_{i, i}$
4.  $\sum_j arc_{i, j} \leq k, \forall i$
    -   $ctr_{i, j - 1, l} \to ctr_{i, j, l}, \forall 1 \leq i \leq n, 1 < j < n, 1 < l \leq (j, k)$
    -   $arc_{i, j} \to ctr_{i, j, 1}, \forall 1 \le i \le n, 1 \le j < n$
    -   $arc_{i, j} \and ctr_{i, j - 1, l - 1} \to ctr_{i j, l}, \forall 1 \leq i \leq n, 1 < j < n, 1 < l \leq (j, k)$
    -   $\neg(arc_{i, j} \and ctr_{i, j - 1, k}), \forall 1 \leq i \leq n, k < j < n$
5.  Add $n(n - 1)(n - 2)/2$ clauses for acceleration.
    -   $\neg arc_{i, j} \or \neg arc_{i, l} \or \neg arc_{j, l} \or \neg arc_{l, j}$



