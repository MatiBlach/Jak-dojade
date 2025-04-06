#include <iostream>
#include "Vector.h"
#include "Queue.h"
#include "Pair.h"

using namespace std;

const int MAX = 30;
const int INF = 1000000;

struct Punkt {
    int x, y;
    Punkt() {
        x = -1;
        y = -1;
    }
    Punkt(const int x, const int y) : x(x), y(y){}

    void set(const int a, const int b) {
        x = a;
        y = b;
    }
};

struct Miasto {
    const char* nazwa = "";
    Punkt punkt;
    Vector<Pair<int, int>> sasiedzi; //indeks + czas

};

Punkt znajdzZnak(char** map, int x, int y, int n, int m) {
    Punkt p;
    int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 }; 
    int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 }; 

    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && map[nx][ny] != '*' && map[nx][ny] != '#' && map[nx][ny] != '.') {
            p.x = nx;
            p.y = ny;
            return p;
        }
    }

    return p;
}

char* znajdzNazweNaMapie(char** map, int i, int j, int n, int m) {
    char* nazwa = new char[MAX];
    Punkt punkt;
    punkt = znajdzZnak(map, i, j, n, m);
    i = punkt.x;
    j = punkt.y;
    int k = j;
    while (k >= 0 && map[i][k] != '.' && map[i][k] != '#' && map[i][k] != '*') {
        k--;
    }
    k++;
    int idx = 0;
    while (k < m && map[i][k] != '.' && map[i][k] != '#' && map[i][k] != '*') {
        nazwa[idx++] = map[i][k++];
    }
    nazwa[idx] = '\0';
    return nazwa;
}

int znajdzIndeksMiasta(Vector<Miasto>& miasta, const char* nazwaMiasta) {
    for (int i = 0; i < (int)miasta.size(); i++) {
        if (strcmp(miasta[i].nazwa, nazwaMiasta) == 0) {
            return i;
        }
    }
    return -1;  
}

const char* znajdzNazweMiasta(Vector<Miasto>& miasta, int indeksMiasta) {
    if (indeksMiasta < 0 || indeksMiasta >= (int)miasta.size()) {
        return nullptr;  
    }
    return miasta[indeksMiasta].nazwa;
}

void bfs(char** map, int n, int m, Vector<Miasto>& miasta, Miasto& miasto) {
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    int** odleglosc = new int* [n];
    for (int i = 0; i < n; i++) {
        odleglosc[i] = new int[m];
        for (int j = 0; j < m; j++) {
            odleglosc[i][j] = -1;
        }
    }

    Queue <Punkt> q;
    q.push({ miasto.punkt.x, miasto.punkt.y });
    odleglosc[miasto.punkt.x][miasto.punkt.y] = 0;

    while (!q.empty()) {
        int cx = q.front().x;
        int cy = q.front().y;
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && odleglosc[nx][ny] == -1) { // odl = -1 -> nieodwiedzony
                odleglosc[nx][ny] = odleglosc[cx][cy] + 1;
                if (map[nx][ny] == '#') {
                    q.push({ nx, ny });
                }
                else if (map[nx][ny] == '*') {
                    for (int j = 0; j < (int)miasta.size(); j++) {
                        if (miasta[j].punkt.x == nx && miasta[j].punkt.y == ny && odleglosc[nx][ny] > 0) {
                            miasto.sasiedzi.push_back({ j, odleglosc[nx][ny] });
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        delete[] odleglosc[i];
    }
    delete[] odleglosc;
}

Pair<int,Vector<int>> dijkstra(Vector<Miasto>& miasta, int start, int koniec) {
    int n = (int)miasta.size();
    Vector<int> odleglosci(n, INF);
    Vector<int> poprzednicy(n, -1);
    Vector<bool> odwiedzone(n, false);
    Queue<int> q;
    q.push(start);
    odleglosci[start] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (odwiedzone[u]) {
            continue;
        }
        odwiedzone[u] = true;
        for (auto v : miasta[u].sasiedzi) {
            int odleglosc = odleglosci[u] + v.second;
            if (odleglosc < odleglosci[v.first]) {
                odleglosci[v.first] = odleglosc;
                poprzednicy[v.first] = u;
                q.push(v.first);
            }
        }
        
    }
    Vector<int> sciezka;
    if (odleglosci[koniec] != INF) {
        int u = koniec;
        while (u != start) {
            sciezka.push_back(u);
            u = poprzednicy[u];
        }
        reverse(sciezka.begin(), sciezka.end());
        sciezka.pop_back();
    }
    return make_Pair(odleglosci[koniec],sciezka);
}

int main() {
    char c;
    char miasto1[MAX] = "";
    char miasto2[MAX] = "";
    int LiczbaMiast;

    int n, m;
    cin >> m >> n;

    char** map = new char* [n]; 
    for (int i = 0; i < n; i++) {
        map[i] = new char[m];
    }

    Vector<Miasto> miasta;

    //wczytywanie mapy
    c = _getchar_nolock();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            map[i][j] = _getchar_nolock();
        }
        c = _getchar_nolock();
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (map[i][j] == '*') {
                Miasto miasto;
                miasto.punkt.set(i, j);
                char* nazwa = znajdzNazweNaMapie(map, i, j, n, m);
                miasto.nazwa = nazwa;
                miasta.push_back(miasto);
            }
        }
    }
    LiczbaMiast = (int)miasta.size();

    //wczytywanie lotow
    int l;
    cin >> l;
    c = _getchar_nolock();
    for (int i = 0; i < l; i++) {
        int k = 0;
        c = _getchar_nolock();
        while (c != ' ') {
            miasto1[k] = c;
            k++;
            c = _getchar_nolock();
        }
        miasto1[k] = '\0';

        k = 0;
        c = _getchar_nolock();
        while (c != ' ') {
            miasto2[k] = c;
            k++;
            c = _getchar_nolock();
        }
        miasto2[k] = '\0';

        int czas;
        cin >> czas;
        c = _getchar_nolock();

        int indeks1 = znajdzIndeksMiasta(miasta, miasto1);
        int indeks2 = znajdzIndeksMiasta(miasta, miasto2);
        if (indeks1 != -1 && indeks2 != -1) {
            miasta[indeks1].sasiedzi.push_back({ indeks2, czas });
        }
    }

    for (auto& miasto : miasta) {
        bfs(map, n, m, miasta, miasto);
    }

    //Wczytywanie zapytan
    int q;
    cin >> q;
    c = _getchar_nolock();
    for (int i = 0; i < q; i++) {
        int k = 0;
        c = _getchar_nolock();
        while (c != ' ') {
            miasto1[k] = c;
            k++;
            c = _getchar_nolock();
        }
        miasto1[k] = '\0';

        k = 0;
        c = _getchar_nolock();
        while (c != ' ') {
            miasto2[k] = c;
            k++;
            c = _getchar_nolock();
        }
        miasto2[k] = '\0';

        bool czySciezka;
        cin >> czySciezka;
        c = _getchar_nolock();

        int start = znajdzIndeksMiasta(miasta, miasto1);
        int koniec = znajdzIndeksMiasta(miasta, miasto2);
        Pair<int,Vector<int>> trasa = dijkstra(miasta, start, koniec);
        cout << trasa.first << " ";

        if (czySciezka) {
            for (int j = 0; j < (int)trasa.second.size(); j++) {
                cout << znajdzNazweMiasta(miasta, trasa.second[j]) << " ";
            }
        }
      
        cout << endl;
    }

    for (int i = 0; i < LiczbaMiast; i++) {
        delete[] miasta[i].nazwa;
    }

    for (int i = 0; i < n; i++) {
        delete[] map[i];
    }
    delete[] map;

    return 0;
}
