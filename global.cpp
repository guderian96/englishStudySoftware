#include "global.h"

global::global()
{
    FILE* fp;
    fp = fopen("list.txt", "r");
    char in[100];
    while (fscanf(fp, "%s", &in) != EOF)
    {
        englishWord ew;
        std::string incs(in);
        ew.word = incs;
        char attr[100];
        fscanf(fp, "%s", &attr);
        std::string attrcs(attr);
        ew.attr = attrcs;
        char mean[100];
        while(fscanf(fp, "%s", &mean)){
            std::string meancs(mean);
            if(meancs == ";")
                break;
            ew.means.push_back((meancs));
        }
        englishWordList_.push_back(ew);
    }
    fclose(fp);
}
