#include "WelcomeWindow.h"
#include "GameWindow.h"
#include <cstdio>
#include <tuple>
#include <string>

static std::tuple<int,int,int> readConfig(){
    int c=25,r=16,m=50;
    if(FILE* f=fopen("files/config.cfg","r")){
        fscanf(f,"%d%d%d",&c,&r,&m);
        fclose(f);
    }
    return std::make_tuple(c,r,m);
}

int main(){
    auto [c,r,m]=readConfig();

    std::string name;
    {
        WelcomeWindow w((unsigned)c,(unsigned)r);
        if(!w.run(name)) return 0;
    }

    GameWindow g((unsigned)c,(unsigned)r,(unsigned)m,name);
    g.run();
    return 0;
}
