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
    vl_l reward;
    vl money;
    vl depend;
};
vector<job>jobs;
vvl pos_to_job;

vvl_l V;
void input(){
    cin>>T_max;
    cin>>N_V;
    {
        ll N_E;cin>>N_E;
        
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
            auto calc=[](vl_l a,ll t){
                vl ret(t+2);
                rep(i,0,(int)a.size()-2){
                    rep(j,a[i].fi,a[i+1].fi){
                        assert(0<=j && j<=t+1);
                        ret[j] = a[i].se + (j-a[i].fi) * (a[i+1].se - a[i].se)/(dd)(a[i+1].fi-a[i].fi);
                    }
                }
                return ret;
            };
            jobs[i].money = calc(jobs[i].reward,T_max);
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

struct one_action{
    ll type;
    //1stay 2move 3execute
    ll pos;
    ll job;
    ll L;
};
typedef vector<one_action> voa;
typedef vector<voa> vvoa;

struct P{
    vvoa vec;
    vl job_res;
    vl worker_pos;
    ll score;
};


P beam_search(){
    vector<P> pres;
    {
        P init;
        init.vec.assign(N_worker + 1,{{}});
        init.job_res.resize(N_job+1);
        rep(i,1,N_job)init.job_res[i] = jobs[i].L;
        init.worker_pos.resize(N_worker + 1);
        rep(i,1,N_worker)init.worker_pos[i] = workers[i].pos;
        init.score = 0;
        pres.pb(init);
    }
    
    rep(time,1,T_max){
        cerr<<time<<endl;
        rep(man,1,N_worker){
            vector<P> nexts;
            for(P pre:pres){
                if((int)pre.vec[man].size()>time){
                    assert(pre.vec[man][time].type == 2);
                    nexts.pb(pre);
                    continue;
                }
                
                for(ll job:pos_to_job[pre.worker_pos[man]]){
                    bool ng=false;
                    if(pre.job_res[job] == 0)ng=true;
                    if(jobs[job].money[time] == 0)ng=true;
                    if(workers[man].type[jobs[job].type] == 0)ng=true;
                    for(ll x:jobs[job].depend)if(pre.job_res[x] > 0)ng=true;
                    if(ng)continue;
                    P next = pre;
                    ll task_amm = min(next.job_res[job],workers[man].L_max);
                    assert(task_amm>0);
                    next.job_res[job] -= task_amm;
                    next.score += task_amm * jobs[job].money[time];
                    next.vec[man].pb({3,-1,job,task_amm});
                    nexts.pb(next);
                }
                for(l_l edge:V[pre.worker_pos[man]]){
                    P next = pre;
                    next.worker_pos[man] = edge.fi;
                    rep(zz,1,edge.se){
                        next.vec[man].pb({2,edge.fi,-1,-1});
                    }
                    nexts.pb(next);
                }
                {
                    P next = pre;
                    next.vec[man].pb({1,-1,-1,-1});
                    nexts.pb(next);
                }
                
            }
            sort(all(nexts),[](P a,P b){return a.score>b.score;});
            while(nexts.size()>100)nexts.pop_back();
            pres = nexts;
        }
    }
    return pres.front();
}
void output(P ans){
    cerr<<"終了まで"<<stop_watch()<<"ms"<<endl;
    DEB(ans.score);
    rep(i,1,T_max){
        rep(j,1,N_worker){
            one_action x = ans.vec[j][i];
            if(x.type==1){
                assert(x.job == -1 && x.pos==-1 && x.L == -1);
                cout<<"stay"<<endl;
            }else if(x.type==2){
                assert(x.job == -1 && x.L ==-1);
                cout<<"move "<<x.pos<<endl;
            }else{
                assert(x.type==3);
                assert(x.pos==-1);
                cout<<"execute "<<x.job<<" "<<x.L<<endl;
            }
        }
    }
    ll ret_score;cin>>ret_score;
    DEB(ret_score);
}

signed main(){fastio
    
    clock_gettime(CLOCK_REALTIME, &START_TIME);
    input();
    
    DEB(T_max);
    DEB(N_job);
    DEB(N_worker);
    
    P ans = beam_search();
    output(ans);
    
    
    return 0;
}
