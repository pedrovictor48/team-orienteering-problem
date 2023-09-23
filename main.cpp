#include <ilcplex/ilocplex.h>
#include <vector>
#include <string>

using namespace std;

vector<vector<int>> adjm, adjp, ak;
int N, M, m;
float L;
vector<int> p;
vector<float> xv, yv, l;

float eucl_dist(int u, int v) {
	float d1 = (xv[u] - xv[v]), d2 = (yv[u] - yv[v]);
	return sqrt(d1*d1 + d2*d2);
}

void read_in() {
	string lixo;
	cin >> lixo >> N;
	cin >> lixo >> m;
	cin >> lixo >> L;
	N -= 2;

	adjm = vector<vector<int>>(N + 2);
	adjp = vector<vector<int>>(N + 2);
	ak = vector<vector<int>>(N + 2, vector<int>(N + 2, -1));

	p = vector<int>(N + 2);
	xv = vector<float>(N + 2);
	yv = vector<float>(N + 2);
	l = vector<float>();

	for(int i = 0; i <= N + 1; i++) {
		cin >> xv[i] >> yv[i] >> p[i];
	}

	//montando as arestas e listas de adj
	int ac = 0; //arco counter
	for(int i = 0; i <= N; i++) {
		for(int j = 1; j <= N + 1; j++) {
			if(i == j) continue;
			//arco (i, j)
			l.push_back(eucl_dist(i, j));
			ak[i][j] = ac++;
			adjp[i].push_back(j);
			adjm[j].push_back(i);
		}
	}
	M = ac;
}

void dfs(int curr, vector<vector<int>>& m, vector<int>& vis, vector<int>& path) {
	for(int next = 1; next <= N; next++) {
		if(m[curr][next] && !vis[next]) {
			vis[next] = 1;
			path.push_back(next);
			dfs(next, m, vis, path);
		}
	}
}

ILOLAZYCONSTRAINTCALLBACK3(SubtourEliminationCallback, IloIntVarArray, y, IloArray<IloIntVarArray>, x, IloNum, tol) {
	IloEnv env = getEnv();
	vector<int> vert_in(N + 1);
	for(int v = 1; v <= N; v++) {
		IloNum in = getValue(y[v]);
		if(in >= 1.0 - tol) vert_in[v] = 1;
	}
	vector<vector<int>> arco_in(N + 2, vector<int>(N + 2));
	for(int k = 0; k < m; k++) {
		for(int i = 0; i <= N + 1; i++) {
			for(int j = 1; j <= N + 1; j++) {
				//checar se aresta existe
				int a = ak[i][j];
				if(a != -1) {
					IloInt in_sol = getValue(x[a][k]);
					if(in_sol >= 1.0 - tol) arco_in[i][j] = 1;
				}
			}
		}
	}

	//agora eu tenho o subgrafo que faz parte da minha sol, sem k, como se todos veiculos fossem 1 so

	vector<int> vis(N + 2);
	for(int st = 1; st <= N; st++) {
		if(vert_in[st] && !vis[st]) {
			vector<int> S;
			dfs(st, arco_in, vis, S);

			//pela minha implementacao do dfs, se vis[st] agora vale 1, eu tenho um ciclo
			if(vis[st]) {
				IloExpr soma(env);
				for(int v : S) {
					for(int incide : adjm[v]) {
						int a = ak[incide][v];
						if(a != -1 && arco_in[incide][v] == 0) {
							for(int k = 0; k < m; k++) soma += x[a][k];
						}
					}
				}

				for(int v : S) add(soma >= y[v]);
			}
		}
	}
}


int main()
{
	// Criando o ambiente
	IloEnv env;
	try
	{

		// Entrada
		read_in();
		cout << L << endl;
		// Modelo
		IloModel TOP(env, "Team OP");

		// Inicializando o objeto cplex
		IloCplex cplex(TOP);


		// Variável de decisão
		IloIntVarArray y(env, N + 1, 0, 1);
		IloArray<IloIntVarArray> x(env, M);
		for(int i = 0; i < M; i++) {
			x[i] = IloIntVarArray(env, m, 0, 1);
		}

		// Função Objetivo
		IloExpr somatorio(env);
		for(int v = 1; v <= N; v++) 
			somatorio += p[v] * y[v];
		TOP.add(IloMaximize(env, somatorio)); // max somatorio

		// Restrição (2)
		for(int v = 1; v <= N; v++) {
			IloExpr somatorio(env);
			for(int k = 0; k < m; k++) {
				for(int u : adjm[v]) {
					int a = ak[u][v];
					somatorio += x[a][k];
				}
			}
			TOP.add(somatorio == y[v]);
		}

		// Restrição (4)
		for(int a = 0; a < M; a++) {
			IloExpr somatorio(env);
			for(int k = 0; k < m; k++) somatorio += x[a][k];
			TOP.add(somatorio <= 1);
		}

		// Restrição (5)
		for(int k = 0; k < m; k++) {
			IloExpr somatorio(env);
			for(int a = 0; a < M; a++) {
				somatorio += (l[a] * x[a][k]);
			}
			TOP.add(somatorio <= L);
		}

		// Restrição (6)
		for(int k = 0; k < m; k++) {
			IloExpr somatorio(env);
			for(int v : adjp[0]) {
				int a = ak[0][v];
				somatorio += x[a][k];
			}
			TOP.add(somatorio == 1);
		}


		// Restrição (7)
		for(int k = 0; k < m; k++) {
			IloExpr somatorio(env);
			for(int v : adjm[N + 1]) {
				int a = ak[v][N + 1];
				somatorio += x[a][k];
			}
			TOP.add(somatorio == 1);
		}

		//A 8 e 9 ficam setadas na definicao das variaveis.
		IloNum tol = cplex.getParam(IloCplex::EpInt);
		IloCplex::Callback sec = cplex.use( SubtourEliminationCallback(env, y, x, tol));

		// Rodando o algoritmo
		if ( cplex.solve() )
			cerr << "Premio ótimo: " << cplex.getObjValue() << endl;

		// Obtendo a solução
		for(int k = 0; k < m; k++) {

			vector<int> prox(N + 2, -1);
			for(int i = 0; i <= N; i++) {
				for(int j = 1; j <= N + 1; j++) {
					int a = ak[i][j];
					if(a != -1) {
						IloNum in = cplex.getValue(x[a][k]);
						if(in >= 1.0 - tol) prox[i] = j;
					}
					else cout << 0 << " ";

				}
			}
			cout << "Carro " << k + 1 << ": ";
			int curr = 0, score = 0;
			float tempo = 0;
			while(curr != N + 1) {
				cout << curr << " ";
				curr = prox[curr];
			}
			cout << N + 1 << endl;
		}
		//Imprimindo a solução

}
	catch (const IloException& e)
	{
		cerr << "Exception caught: " << e << endl;
	}
	catch (...)
	{
		cerr << "Unknown exception caught!" << endl;
	}

	env.end();
	return 0;
}

