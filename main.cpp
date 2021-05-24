#include <bits/stdc++.h>
using namespace std;

vector<vector<bool>> read(char file[], int &k) {
  FILE *fp = freopen(file, "r", stdin);
  int n, m;
  cin >> n >> m >> k;
  vector<vector<bool>> ret(n, vector<bool>(n));
  for (int i = 0, a, b; i < m; i++) {
    cin >> a >> b;
    ret[a][b] = ret[b][a] = true;
  }
  fclose(fp);
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
  // Triangulation
  vector<vector<int>> clauses;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (!G[i][j])
        continue;
      clauses.emplace_back(vector<int>{-ord(i, j), arc(i, j)});
      clauses.emplace_back(vector<int>{ord(i, j), arc(j, i)});
    }
  }
  // Transtivity
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
            vector<int>{-arc(i, j), -arc(i, l), ord(j, l), arc(j, l)});
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
  FILE *fp = freopen("tmp.in", "r", stdin);
  cout << "c\np cnf " << n * n * (k + 2) << ' ' << clauses.size() << '\n';
  for (auto clause : clauses) {
    for (int i : clause)
      cout << i << ' ';
    cout << "0\n";
  }
  fclose(fp);
}

bool exeMiniSAT(char MiniSAT[]) {
  char cmd[300] = "";
  strcpy(cmd, MiniSAT);
  strcat(cmd, " tmp.in tmp.out");
  system(cmd);
  FILE *fp = freopen("tmp.out", "r", stdin);
  string str;
  cin >> str;
  fclose(fp);
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
  FILE *fp = freopen(argv[2], "w", stdout);
  cout << (SAT ? "YES" : "NO") << '\n';
  fclose(fp);
  return 0;
}