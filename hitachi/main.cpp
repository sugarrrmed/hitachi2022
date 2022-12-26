#ifdef LOCAL

#else
#pragma GCC target("avx2")
#endif

#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

//#include<atcoder/all>
#include<bits/stdc++.h>
using namespace std;
//using namespace atcoder;
typedef long long ll;
//typedef int ll;

typedef long double dd;
typedef pair<ll,ll> l_l;
typedef pair<dd,dd> d_d;
ll inf=(ll)1E18;
#define rep(i,l,r) for(ll i=l;i<=r;i++)
#define rrep(i,r,l) for(ll i=r;i>=l;i--)
#define pb push_back
ll max(ll a,ll b){if(a<b)return b;else return a;}
ll min(ll a,ll b){if(a>b)return b;else return a;}
dd EPS=1E-12;
#define fi first
#define se second

#define SORT(v) sort(v.begin(),v.end())
#define ERASE(v) sort(v.begin(),v.end()); v.erase(unique(v.begin(),v.end()),v.end())
#define POSL(v,x) (lower_bound(v.begin(),v.end(),x)-v.begin())
#define POSU(v,x) (upper_bound(v.begin(),v.end(),x)-v.begin())
template<class T,class S>
inline bool chmax(T &a, S b) {
    if(a < b) {
        a = (T)b;
        return true;
    }
    return false;
}
template<class T,class S>
inline bool chmin(T &a, S b) {
    if(a > b) {
        a = (T)b;
        return true;
    }
    return false;
}
#define all(c) c.begin(),c.end()

typedef vector<ll> vl;
typedef vector<vl> vvl;
typedef vector<vvl>vvvl;
typedef vector<vvvl>vvvvl;
typedef vector<l_l>vl_l;
typedef vector<vl_l>vvl_l;
typedef vector<vvl_l>vvvl_l;
typedef vector<string>vs;
typedef vector<vs>vvs;
typedef vector<dd> vd;
typedef vector<vd> vvd;
typedef vector<vvd> vvvd;
typedef vector<d_d>vd_d;
dd PI=acos((dd)-1);

#define fastio ios::sync_with_stdio(false); cin.tie(0); cout.tie(0); cout<<setprecision(10); cerr<<setprecision(10); cerr<<fixed;
template <class T> using pq = priority_queue<T>;
template <class T> using pqg = priority_queue<T, vector<T>, greater<T>>;

//#define endl "\n"  //インタラクティブで消す！！！！！！！！！！！！！！！！！！！！！
#define popcount __builtin_popcountll
#define SHUFFLE(v) shuffle(all(v),default_random_engine(chrono::system_clock::now().time_since_epoch().count()))
//////////////////////////////////////////////////////////////////////////////
template<class S>
void DEBUG_PRINT(S x){
    cerr<<x<<endl;
}
template<class S,class T>
void DEBUG_PRINT(pair<S,T>x){
    cerr<<"("<<x.fi<<","<<x.se<<")"<<endl;
}
template<class S>
void DEBUG_PRINT(vector<S> x){
    for(auto y:x)cerr<<setw(2)<<y<<" ";
    cerr<<endl;
}
template<class S,class T>
void DEBUG_PRINT(vector<pair<S,T>>x){
    for(auto y:x)cerr<<"("<<y.fi<<","<<y.se<<") ";
    cerr<<endl;
}
template<class S>
void DEBUG_PRINT(vector<vector<S>> x){
    cerr<<endl;
    for(auto y:x){
        for(auto z:y){
            cerr<<setw(2)<<z<<" ";
        }cerr<<endl;
    }cerr<<endl;
}

#define TO_STRING_FOR_DEBUG(VariableName) # VariableName
#ifdef LOCAL
#define DEB(V) cerr<< TO_STRING_FOR_DEBUG(V) << ": "; DEBUG_PRINT(V);
#else
#define DEB(V) void(0)
#endif

#define importantBit(x) (63-__builtin_clzll(x))
/////////////////////////////////////////////////////////////////////////////////////////

//*
unsigned int randxor(){
    //0 以上 (2^32)-1 以下の整数(だと思う)
    static unsigned int x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned int t;
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}
//*/
/*
random_device rd;
ll randxor(){
    mt19937 mt(rd());
    return mt();
}
*/

ll rand(ll a,ll b){
    //a以上b以下の整数を返す。
    assert(a<=b);
    ll len=(b-a+1);
    return randxor()%len + a;
}
dd randdouble(dd l,dd r){
    //l以上r以下の実数を返す。
    assert(l<=r + EPS);
    return l + (r-l) * (dd)randxor()/4294967295;
}
struct timespec START_TIME;
dd stop_watch() {
    // 経過時間をミリ秒単位で返す.
    struct timespec end_time;
    clock_gettime(CLOCK_REALTIME, &end_time);
    ll sec = end_time.tv_sec - START_TIME.tv_sec;
    ll nsec = end_time.tv_nsec - START_TIME.tv_nsec;
    return 1000 * (dd)sec + (dd)nsec / (1000000);
}

////////////////////////////////////////////////

ll T_max;
ll N_V;
vvl DIST;
ll N_worker;
struct worker{
    ll pos;
    ll L_max;
    vl type;
};
vector<worker>workers;
ll N_job;
struct job{
    ll id;
    ll type;
    ll L;//問題文だとNだけど、Lに変えた。
    ll pos;
   // vl reward;
    vl_l reward;
    vl depend;
};
vector<job>jobs;
vvl pos_to_job;


void input(){
    cin>>T_max;
    cin>>N_V;
    {
        ll N_E;cin>>N_E;
        vvl_l V(N_V+1);
        V.resize(N_V+1);
        rep(i,1,N_E){
            ll a,b,d;cin>>a>>b>>d;
            V[a].pb({b,d});
            V[b].pb({a,d});
        }
        {
            auto calc_dist=[&](ll start){
                pqg<l_l>q;
                vl dist(N_V+1,inf);
                auto push=[&](ll pos,ll dis){
                    if(chmin(dist[pos],dis)){
                        q.push({dis,pos});
                    }
                };
                push(start,0);
                while(!q.empty()){
                    l_l t=q.top();q.pop();
                    if(dist[t.se] != t.fi)continue;
                    for(l_l edge:V[t.se]){
                        push(edge.fi,edge.se + t.fi);
                    }
                }
                return dist;
            };
            DIST.resize(N_V+1);
            rep(i,1,N_V){
                DIST[i] = calc_dist(i);
            }
        }
    }
    cin>>N_worker;
    workers.resize(N_worker+1);
    rep(i,1,N_worker){
        workers[i].type.resize(4);
        cin>>workers[i].pos >> workers[i].L_max;
        ll Z;cin>>Z;
        while(Z--){
            ll type;cin>>type;
            assert(workers[i].type[type] == 0);
            workers[i].type[type] = 1;
        }
    }
    cin>>N_job;
    jobs.resize(N_job+1);
    rep(i,1,N_job){
        cin>>jobs[i].id>>jobs[i].type>>jobs[i].L>>jobs[i].pos;
        {
            ll N;cin>>N;
            jobs[i].reward.resize(N);
            rep(j,0,N-1){
                cin>>jobs[i].reward[j].fi>>jobs[i].reward[j].se;
            }
        }
        {
            ll N;cin>>N;
            jobs[i].depend.resize(N);
            rep(j,0,N-1){
                cin>>jobs[i].depend[j];
            }
        }
    }
    
    pos_to_job.resize(N_V+1);
    rep(i,1,N_job){
        pos_to_job[jobs[i].pos].pb(i);
    }
}

struct action{
    ll mati;
    ll job;
    ll time;//jobの実行時間
};
typedef vector<action> va;
typedef vector<va> vva;
struct Ans{
    vva vec;
    ll score = 0;
};

void output(Ans ans){
    vl res_job(N_job+1);
    rep(i,1,N_job)res_job[i] = jobs[i].L;
    
    vl movetime(N_worker + 1);
    vl pos(N_worker + 1);
    rep(i,1,N_worker)pos[i] = workers[i].pos;
    rep(t_time,1,T_max){
        
        rep(z,1,N_worker){
            auto &v = ans.vec[z];
            while(!v.empty() && v[0].mati == 0 && v[0].time == 0){
                v.erase(v.begin());
            }
            if(v.empty()){
                cout<<"stay"<<endl;
                continue;
            }
            if(movetime[z] == 0 && pos[z] != jobs[v[0].job].pos){
                movetime[z] = DIST[pos[z]][jobs[v[0].job].pos];
            }
            if(movetime[z] > 0){
                movetime[z]--;
                cout<<"move "<<jobs[v[0].job].pos<<endl;
                pos[z] = jobs[v[0].job].pos;
                continue;
            }
            if(v[0].mati > 0){
                v[0].mati--;
                cout<<"stay"<<endl;
                continue;
            }
            assert(v[0].time > 0);
            
            //前から貪欲な仕事数だけ実行してる（明らかに不適切）
            ll L = min(workers[z].L_max,res_job[v[0].job]);
            if(L==0){
                cerr<<"正しいか確認して "<<t_time <<" "<<z<<endl;
                cout<<"stay"<<endl;
            }else{
                cout<<"execute "<<v[0].job<<" "<<L<<endl;
                res_job[v[0].job] -= L;
                v[0].time --;
            }
        }
    }
    
    ll return_score;cin>>return_score;
    DEB(return_score);
    exit(0);
}

ll roundup(ll a,ll b){
    assert(a>=0 && b>0);
    return (a+b-1)/b;
}

signed main(){fastio
    
    input();
    
    Ans ans;
    ans.vec.resize(N_worker+1);
    
    vl used_job(N_job+1);
    rep(i,1,N_worker){
        vl start_time(N_job + 1, inf);
        rep(j,1,N_job){
            bool ng=false;
            if(!jobs[j].depend.empty())ng=true;
            if(workers[i].type[jobs[j].type] == 0)ng=true;
            if(used_job[j] == 1)ng=true;
            
            if(ng)continue;
            
            ll req_time = roundup(jobs[j].L,workers[i].L_max);
            ll move_time = DIST[workers[i].pos][jobs[j].pos];
            if(move_time + req_time < jobs[j].reward.back().fi){
                chmin(start_time[j], max(move_time, jobs[j].reward[0].fi+1));
            }
        }
        {
            ll posj = 0;
            rep(j,1,N_job){
                if(start_time[j] < start_time[posj]){
                    posj = j;
                }
            }
            assert(posj != 0);
            ll req_time = roundup(jobs[posj].L,workers[i].L_max);
            ll move_time = DIST[workers[i].pos][jobs[posj].pos];
            ans.vec[i].pb({start_time[posj] - move_time ,posj ,req_time});
            
            assert(used_job[posj] == 0);
            used_job[posj] = 1;
        }
    }
    
    output(ans);
    
    return 0;
}
