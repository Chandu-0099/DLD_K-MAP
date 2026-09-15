#include <iostream>
#include <string>
using namespace std;

class KMap {
private:
    int a[4][4], g[20], n=0;
    string term(int x);
    int lit(int x);
public:
    void input();
    void solve();
};

string KMap::term(int x) {
    char v[]={'a','b','c','d'};
    string s="";
    for(int b=3;b>=0;b--) {
        int x1=-1, same=1;
        for(int m=0;m<16;m++) {
            if(x&(1<<m)) {
                int z=(m>>b)&1;
                if(x1==-1) {
                    x1=z;
                }
                else if(x1!=z) {
                    same=0;
                }
            }
        }
        if(same) {
            s+=v[3-b];
            if(x1==0) {
                s+="'";
            }
        }
    }
    return s;
}

int KMap::lit(int x) {
    int c=0;
    for(int b=0;b<4;b++) {
        int z=-1, same=1;
        for(int m=0;m<16;m++) {
            if(x&(1<<m)) {
                int y=(x>>b)&1;
                if(z==-1) {
                    z=y;
                }
                else if(z!=y) {
                    same=0;
                }
            }
        }
        if(same) {
            c++;
        }
    }
    return c;
}

void KMap::input() {
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            cin>>a[i][j];
        }
    }
}

void KMap::solve() {
    int p[]={0,1,3,2};

    // Generate groups
    for(int r=0;r<4;r++) {
        for(int c=0;c<4;c++) {
            for(int h=1;h<=4;h*=2) {
                for(int w=1;w<=4;w*=2) {
                    int x=0, ok=1;
                    for(int i=0;i<h;i++) {
                        for(int j=0;j<w;j++) {
                            int R=(r+i)%4;
                            int C=(c+j)%4;
                            if(!a[R][C]) {
                                ok=0;
                            }
                            x|=1<<((p[R]<<2)|p[C]);
                        }
                    }
                    if(ok) {
                        bool f=0;
                        for(int i=0;i<n;i++) {
                            if(g[i]==x) {
                                f=1;
                            }
                        }
                        if(!f) {
                            g[n++]=x;
                        }
                    }
                }
            }
        }
    }

    // Prime implicants
    int q[20], k=0;
    for(int i=0;i<n;i++) {
        bool small=0;
        for(int j=0;j<n;j++) {
            if(i!=j && (g[i]&g[j])==g[i]) {
                small=1;
            }
        }
        if(!small) {
            q[k++]=g[i];
        }
    }

    n=k;
    for(int i=0;i<n;i++) {
        g[i]=q[i];
    }

    // Find minimum terms, then literals
    int BT=100, BL=100;
    for(int s=1;s<(1<<n);s++) {
        int t=0,l=0;
        for(int i=0;i<n;i++) {
            if(s&(1<<i)) {
                t++;
                l+=lit(g[i]);
            }
        }
        if(t>BT || (t==BT && l>=BL)) {
            continue;
        }
        bool ok=1;
        for(int r=0;r<4;r++) {
            for(int c=0;c<4;c++) {
                if(a[r][c]) {
                    int m=(p[r]<<2)|p[c];
                    bool f=0;
                    for(int i=0;i<n;i++) {
                        if((s&(1<<i)) && (g[i]&(1<<m))) {
                            f=1;
                        }
                    }
                    if(!f) {
                        ok=0;
                    }
                }
            }
        }
        if(ok) {
            BT=t;
            BL=l;
        }
    }

    cout<<"\nAll possible minimized expressions:\n";

    // Print all minimum expressions
    for(int s=1;s<(1<<n);s++) {
        int t=0,l=0;
        for(int i=0;i<n;i++) {
            if(s&(1<<i)) {
                t++;
                l+=lit(g[i]);
            }
        }
        if(t!=BT || l!=BL) {
            continue;
        }
        bool ok=1;
        for(int r=0;r<4;r++) {
            for(int c=0;c<4;c++) {
                if(a[r][c]) {
                    int m=(p[r]<<2)|p[c];
                    bool f=0;
                    for(int i=0;i<n;i++) {
                        if((s&(1<<i)) && (g[i]&(1<<m))) {
                            f=1;
                        }
                    }
                    if(!f) {
                        ok=0;
                    }
                }
            }
        }
        if(ok) {
            bool first=1;
            for(int i=0;i<n;i++) {
                if(s&(1<<i)) {
                    if(!first) {
                        cout<<" + ";
                    }
                    cout<<term(g[i]);
                    first=0;
                }
            }
            cout<<endl;
        }
    }
};

int main() {
    KMap k;
    cout<<"Enter 4x4 K-map:\n";
    k.input();
    k.solve();
    return 0;
}