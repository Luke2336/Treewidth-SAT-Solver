#include <bits/stdc++.h>
using namespace std;

vector<vector<bool>> read(char file[], int &k) {
  fstream fin;
  fin.open(file, ios::in | ios::binary);
  int n, m;
  fin >> n >> m >> k;
  vector<vector<bool>> ret(n, vector<bool>(n));
  for (int i = 0, a, b; i < m; i++) {
    fin >> a >> b;
    ret[a][b] = ret[b][a] = true;
  }
  fin.close();
  return ret;
}

vector<vector<int>> genClauses(const vector<vector<bool>> G, const int k) {
  int n = G.size();
  // Encode variable
  auto ord = [&](int i, int j) { return i * n + j + 1; };
  auto arc = [&](int i, int j) { return i * n + j + n * n + 1; };
  auto ctr = [&](int i, int j, int l) {
    return (l - 1) * n * n + i * n + j + 1 + 2 * n * n;
  };
  vector<vector<int>> clauses;
  // Transtivity
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j)
        continue;
      for (int l = 0; l < n; l++) {
        if (i == l || j == l)
          continue;
        int ij = (i < j) ? ord(i, j) : -ord(j, i);
        int jl = (j < l) ? ord(j, l) : -ord(l, j);
        int il = (i < l) ? ord(i, l) : -ord(l, i);
        clauses.emplace_back(vector<int>{-ij, -jl, il});
      }
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (!G[i][j])
        continue;
      clauses.emplace_back(vector<int>{-ord(i, j), arc(i, j)});
      clauses.emplace_back(vector<int>{ord(i, j), arc(j, i)});
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j)
        continue;
      for (int l = j + 1; l < n; l++) {
        if (i == l)
          continue;
        clauses.emplace_back(
            vector<int>{-arc(i, j), -arc(i, l), -ord(j, l), arc(j, l)});
        clauses.emplace_back(
            vector<int>{-arc(i, j), -arc(i, l), ord(j, l), arc(l, j)});
      }
    }
  }
  // Neglect sel-loops
  for (int i = 0; i < n; i++) {
    clauses.emplace_back(vector<int>{-arc(i, i)});
  }
  // Count
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      clauses.emplace_back(vector<int>{-arc(i, j), ctr(i, j, 1)});
    for (int j = 1; j < n; j++) {
      for (int l = 2; l <= min(j, k); l++) {
        clauses.emplace_back(vector<int>{-ctr(i, j - 1, l), ctr(i, j, l)});
        clauses.emplace_back(
            vector<int>{-arc(i, j), -ctr(i, j - 1, l - 1), ctr(i, j, l)});
      }
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = k + 1; j < n; j++)
      clauses.emplace_back(vector<int>{-arc(i, j), -ctr(i, j - 1, k)});
  }
  // Accelerate
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      for (int l = 0; l < n; l++)
        clauses.emplace_back(
            vector<int>{-arc(i, j), -arc(i, l), -arc(j, l), -arc(l, j)});
  return clauses;
}

void writeCNF(int n, int k, vector<vector<int>> clauses) {
  fstream fout;
  fout.open("tmp.in", ios::out | ios::binary);
  fout << "c\np cnf " << n * n * (k + 2) << ' ' << clauses.size() << '\n';
  for (auto clause : clauses) {
    for (int i : clause)
      fout << i << ' ';
    fout << "0\n";
  }
  fout.close();
}

bool exeMiniSAT(char MiniSAT[]) {
  char cmd[300] = "";
  strcpy(cmd, MiniSAT);
  strcat(cmd, " tmp.in tmp.out");
  system(cmd);
  fstream fin;
  fin.open("tmp.out", ios::in | ios::binary);
  string str = "";
  while (str != "SAT" && str != "UNSAT")
    fin >> str;
  fin.close();
  cerr << "str = " << str << "\n";
  return str == "SAT";
}

int main(int argc, char *argv[]) {
  if (argc < 4)
    return 0;
  int k;
  auto G = read(argv[1], k);
  auto Clauses = genClauses(G, k);
  writeCNF(G.size(), k, Clauses);
  bool SAT = exeMiniSAT(argv[3]);
  fstream fout;
  fout.open(argv[2], ios::in | ios::binary);
  fout << (SAT ? "YES" : "NO") << '\n';
  fout.close();
  return 0;
}
