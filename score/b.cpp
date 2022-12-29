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
/*
void output(Ans ans){
 
    vvvl print(N_worker + 1,vvl(T_max+1));
    rep(man,1,N_worker){
        ll time = 1;
        ll pre_pos = workers[man].pos;
        for(action x:ans.vec[man]){
            if(x.mati == inf){
                //最後のflag
                while(time<=T_max)print[man][time++] ={1};
                break;
            }
            ll dist = DIST[pre_pos][jobs[x.job].pos];
            rep(_,1,dist){
                assert(time<=T_max);
                print[man][time++] = {2,jobs[x.job].pos};
            }pre_pos = jobs[x.job].pos;
            
            rep(_,1,x.mati){
                assert(time<=T_max);
                print[man][time++] = {1};
            }
            
            ll res_job = jobs[x.job].L;
            rep(_,1,x.time){
                assert(time<=T_max);
                ll L = min(res_job,workers[man].L_max);
                res_job -= L;
                assert(L>0);
                print[man][time++] = {3,x.job,L};
            }
        }
        while(time<=T_max)print[man][time++]={1};
    }
    {
        vl cnt_job(N_job+1);
        
        rep(man,1,N_worker){
            rep(time,1,T_max){
                vl v = print[man][time];
                if(v[0] == 3){
                    cnt_job[v[1]] += v[2];
                }
            }
        }
        
        rep(job,1,N_job){
            if(cnt_job[job] > jobs[job].L)cerr<<job<<" "<<cnt_job[job]<<" "<<jobs[job].L<<endl;
        }
    }
    
    ll SCORE = 0;
    rep(time,1,T_max){
        rep(man,1,N_worker){
            vl p = print[man][time];
            assert((int)p.size()>=1);
            if(p[0]==1){
                cout<<"stay";
            }else if(p[0] == 2){
                cout<<"move ";
            }else if(p[0] == 3){
                cout<<"execute ";
                if(jobs[p[1]].money[time] == 0){
                    DEB(time);
                    DEB(man);
                    DEB(p);
                    DEB(jobs[p[1]].reward);
                }
                assert(jobs[p[1]].money[time]>0);
                SCORE += jobs[p[1]].money[time] * p[2];
            }else{
                assert(0);
            }
            assert((int)p.size()==p[0]);
            rep(i,1,(int)p.size()-1){
                cout<<p[i]<<" ";
            }cout<<endl;
        }
    }
    ll ret_score;cin>>ret_score;
    DEB(ans.score);
    DEB(SCORE);
    DEB(ret_score);
    dd log_score = log10(ret_score);
    DEB(log_score);
}
*/
void output(Ans ans){
    vl res_job(N_job+1);
    rep(i,1,N_job)res_job[i] = jobs[i].L;
    
    vl movetime(N_worker + 1);
    vl pos(N_worker + 1);
    rep(i,1,N_worker)pos[i] = workers[i].pos;
    
    ll SCORE = 0;
    
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
            if(v[0].mati == inf){
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
                
                if(jobs[v[0].job].money[t_time] == 0){
                    cerr<<t_time<<" "<<z<<" "<<v[0].job<<" "<<L<<endl;
                }
                assert(jobs[v[0].job].money[t_time] > 0);
                res_job[v[0].job] -= L;
                v[0].time --;
                SCORE += jobs[v[0].job].money[t_time] * L;
            }
        }
    }
    
    ll ret_score;cin>>ret_score;
    DEB(ans.score);
    DEB(SCORE);
    DEB(ret_score);
    dd log_score = log10(ret_score);
    DEB(log_score);
}
ll roundup(ll a,ll b){
    assert(a>=0 && b>0);
    return (a+b-1)/b;
}


Ans beamsearch(){
    typedef pair<ll,dd> l_d;
    typedef vector<l_d> vl_d;
    struct Data{
        vva vec;
        dd score;
        vl job_endtime;
        vl res_job;
        vl_d add_score;
    };
    
    vector<Data>pres;
    {
        Data init;
        init.vec.resize(N_worker+1);
        init.score = 0;
        init.job_endtime.assign(N_job+1,inf);
        init.add_score.assign(N_worker+1,{0,0});
        init.res_job.resize(N_job+1);
        rep(i,1,N_job)init.res_job[i] = jobs[i].L;
        pres.pb(init);
    }
    
    rep(time,1,T_max){
        rep(man,1,N_worker){
            vector<Data>nexts;
            for(Data pre:pres){
                if(pre.add_score[man].fi>0){
                    Data next = pre;
                    next.add_score[man].fi--;
                    next.score += next.add_score[man].se;
                    nexts.pb(next);
                    continue;
                }
                if(!pre.vec[man].empty() && pre.vec[man].back().mati == inf){
                    nexts.pb(pre);
                    continue;
                }
                
                ll pre_pos;
                if(pre.vec[man].empty())pre_pos = workers[man].pos;
                else pre_pos = jobs[pre.vec[man].back().job].pos;
                
                bool not_found = true;
                rep(job,1,N_job){
                    bool ng=false;
                    if(pre.job_endtime[job]<inf)ng=true;
                    if(workers[man].type[jobs[job].type] == 0)ng=true;
                    
                    ll move_time = DIST[pre_pos][jobs[job].pos];
                    ll start_time = max(time + move_time,jobs[job].reward.front().fi + 1);
                    ll mati_time = start_time - (time + move_time) ;
                    ll req_time = roundup(pre.res_job[job],workers[man].L_max);
                    ll end_time = start_time + req_time - 1;
                    if(end_time >= jobs[job].reward.back().fi)ng=true;
                    
                    for(ll x:jobs[job].depend){
                        if(pre.job_endtime[x] >= time)ng=true;
                    }
                    if(ng)continue;
                    Data next = pre;
                    ll money_all = 0;
                    {
                        rep(z,start_time,end_time){
                            ll use_L = min(next.res_job[job],workers[man].L_max);
                            money_all += jobs[job].money[z] * use_L;
                            assert(use_L > 0);
                            assert(jobs[job].money[z]>0);
                            next.res_job[job] -= use_L;
                        }
                        assert(next.res_job[job] == 0);
                    }
                    ll all_time = end_time - time + 1;
                    dd money_ave = (dd)money_all / all_time;
                    
                    next.vec[man].pb({mati_time,job,req_time});
                    next.score += money_ave;
                    next.job_endtime[job] = end_time;
                    next.add_score[man] = {all_time-1,money_ave};
                    next.res_job[job] = 0;
                    nexts.pb(next);
                    not_found = false;
                }
                if(not_found){
                    rep(job,1,N_job){
                        bool ng=false;
                        if(pre.job_endtime[job]<inf)ng=true;
                        if(workers[man].type[jobs[job].type] == 0)ng=true;
                        
                        ll move_time = DIST[pre_pos][jobs[job].pos];
                        ll start_time = max(time + move_time,jobs[job].reward.front().fi + 1);
                        ll mati_time = start_time - (time + move_time) ;
                        ll end_time = start_time + roundup(pre.res_job[job],workers[man].L_max) -1;
                        chmin(end_time,T_max);
                        chmin(end_time,jobs[job].reward.back().fi-1);
                        ll req_time = end_time - start_time + 1;
                        if(req_time <= 0)ng=true;
                        for(ll x:jobs[job].depend){
                            if(pre.job_endtime[x] >= time)ng=true;
                        }
                        if(ng)continue;
                        Data next = pre;
                        ll money_all = 0;
                        {
                            rep(z,start_time,end_time){
                                ll use_L = min(next.res_job[job],workers[man].L_max);
                                money_all += jobs[job].money[z] * use_L;
                                assert(use_L > 0);
                                assert(jobs[job].money[z]>0);
                                next.res_job[job] -= use_L;
                            }
                            if(next.res_job[job] == 0){
                                next.job_endtime[job] = end_time;
                            }
                        }
                        ll all_time = end_time - time + 1;
                        dd money_ave = (dd)money_all / all_time;
                        next.vec[man].pb({mati_time,job,req_time});
                        next.score += money_ave;
                        next.add_score[man] = {all_time-1,money_ave};
                        nexts.pb(next);
                        not_found = false;
                    }
                }
                {
                    Data next = pre;
                    next.vec[man].pb({inf,-1,-1});
                    nexts.pb(next);
                }
            }
            sort(all(nexts),[](Data a,Data b){return a.score > b.score;});
            while((int)nexts.size()>2)nexts.pop_back();
            pres = nexts;
        }
    }
    {
        Ans ret;
        ret.vec = pres.front().vec;
        ret.score = pres.front().score;
        return ret;
    }
}

ll calc_score(vvl ans,bool end_print){
    vl pos(N_worker+1);
    rep(i,1,N_worker)pos[i] = workers[i].pos;
    vl res_job(N_job+1);
    rep(i,1,N_job)res_job[i] = jobs[i].L;
    vl end_job(N_job+1);
    vl score_job(N_job+1);
    
    vvvl print(T_max+1,vvl(N_worker+1));
    rep(time,1,T_max){
        vl ends;
        rep(man,1,N_worker){
            while(1){
                if(!print[time][man].empty())break;
                if(ans[man].empty() || ans[man].front() == -1){
                    print[time][man] = {1};
                    break;
                }
                ll job = ans[man].front();
                if(pos[man] != jobs[job].pos){
                    ll dist = DIST[pos[man]][jobs[job].pos];
                    pos[man] = jobs[job].pos;
                    rep(z,time,min(T_max,time+dist-1)){
                        print[z][man] = {2,pos[man]};
                    }
                    break;
                }
                assert(workers[man].type[jobs[job].type] == 1);
                if(res_job[job] == 0){
                    ans[man].erase(ans[man].begin());
                    continue;
                }
                bool ng=false;
                if(jobs[job].money[time] == 0){
                    if(time <= jobs[job].reward.front().fi){
                        ng=true;
                    }else{
                        assert(time >= jobs[job].reward.back().fi);
                        ans[man].erase(ans[man].begin());
                        continue;
                    }
                }
                for(ll x:jobs[job].depend){
                    if(end_job[x] == 0)ng=true;
                }
                if(ng){
                    print[time][man] = {1};
                    break;
                }
                ll L = min(workers[man].L_max,res_job[job]);
                assert(L>0);
                ll money = jobs[job].money[time];
                assert(time>0);
                score_job[job] += L*money;
                print[time][man] = {3,job,L};
                res_job[job] -= L;
                if(res_job[job] == 0){
                    ends.pb(job);
                }
            }
        }
        for(ll x:ends){
            assert(end_job[x] == 0);
            end_job[x] = 1;
        }
    }
    
    ll score = 0;
    rep(job,1,N_job){
        if(end_job[job]){
            score += score_job[job];
        }
    }
    
    if(end_print){
        rep(time,1,T_max){
            rep(man,1,N_worker){
                vl p = print[time][man];
                assert(!p.empty());
                assert(p[0] == (int)p.size());
                rep(i,0,(int)p.size()-1){
                    if(i==0){
                        if(p[0] == 1)cout<<"stay ";
                        else if(p[0]==2)cout<<"move ";
                        else cout<<"execute ";
                    }else{
                        cout<<p[i]<<" ";
                    }
                }cout<<endl;
            }
        }
        DEB(score);
        ll ret_score;cin>>ret_score;
        DEB(ret_score);
        dd log_score = log10(ret_score);
        DEB(log_score);
    }
    return score;
}

signed main(){fastio
    clock_gettime(CLOCK_REALTIME, &START_TIME);
    
    
    input();
    DEB(T_max);
    DEB(N_job);
    DEB(N_worker);
    
    Ans ans = beamsearch();
    
    
  //  output(ans);
    
    vvl now_ans(N_worker+1);
    rep(man,1,N_worker){
        for(action x:ans.vec[man]){
            now_ans[man].pb(x.job);
        }
    }
    
    vl now_flag(N_job+1);
    for(vl x:now_ans)for(ll y:x)now_flag[y]++;
    
    ll now_score = calc_score(now_ans,false);
    rep(loop,1,1e9){
        if(stop_watch() >= 4800){
            cerr<<"break loop is "<<loop<<endl;
            break;
        }
        vvl next_ans = now_ans;
        while(1){
            /*
             ll man = rand(1,N_worker);
            ll jobpos = rand(0,(int)now_ans[man].size()-1);
            ll new_job = rand(1,N_job);
            if(now_flag[new_job] >= 1)continue;
            if(workers[man].type[jobs[new_job].type] == 0)continue;
            ll pre_job = next_ans[man][jobpos];
            if(pre_job == new_job)continue;
            next_ans[man][jobpos] = new_job;
            ll next_score = calc_score(next_ans,false);
            if(now_score < next_score){
                now_score = next_score;
                now_ans = next_ans;
                now_flag[pre_job] --;
                now_flag[new_job] ++;
                cerr<<setw(5)<<loop<<" "<<now_score<<endl;
            }
            break;
             //*/
            //*
            ll man1 = rand(1,N_worker);
            ll man2 = rand(1,N_worker);
            ll jobpos1 = rand(0,(int)next_ans[man1].size()-1);
            ll jobpos2 = rand(0,(int)next_ans[man2].size()-1);
            ll job1 = next_ans[man1][jobpos1];
            ll job2 = next_ans[man2][jobpos2];
            if(workers[man1].type[jobs[job2].type] == 0)continue;
            if(workers[man2].type[jobs[job1].type] == 0)continue;
            next_ans[man1][jobpos1] = job2;
            next_ans[man2][jobpos2] = job1;
            ll next_score = calc_score(next_ans,false);
            if(now_score < next_score){
                now_score = next_score;
                now_ans = next_ans;
                cerr<<setw(5)<<loop<<" "<<now_score<<endl;
            }
            break;
             //*/
        }
    }
    
    cerr<<stop_watch()<<"ms"<<endl;
    calc_score(now_ans,true);
    
    return 0;
}

//mean 11.458033133633839
