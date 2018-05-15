// Load the PMT hits, including the nPE and first hit time.
// The limit is 100 events.
// Created by zhuj on 2018/4/24.
//

#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TF1.h>

#include <TMath.h>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;
using namespace TMath;


class HitData {
    
public:
    int event = -1;
    int pmtID = -1;
    double hitTime = -1;
    int hitNum = -1;
    
    bool operator<(const HitData &str) const {
        return (hitTime < str.hitTime);
    }
    
};

vector<HitData> allHitData;


int LoadSortedPMTHit() {
    //gROOT->ProcessLine(".L $SIMEVENTV2ROOT/$CMTCONFIG/libSimEventV2.so");
    TFile *f = TFile::Open("../sample_detsim_user.root");
    if(f==NULL) {
        cout<<"Can not sample_detsim_user.root in ../"<<endl;
        gROOT->ProcessLine(".q");
    }
    f->ls();
    gDirectory->ls();
    TTree *tr = (TTree *) (gDirectory->Get("evt"));
    //tr->Print();
    
    Int_t eveNum;
    Int_t nPhotons;
    tr->SetBranchAddress("nPhotons", &nPhotons);
    tr->GetEntry(1);
    eveNum = tr->GetEntries();
    if(eveNum>100) eveNum =100; //the limit size
    
    
    
    const int number = eveNum;         //this ways to define array do not work when using root to compile.
    static Int_t id[2000002];          //2 millions seems to be the maximum in the user.root
    static Double_t hitTime[2000002];
    
    if (nPhotons > 2000002) cout << "----Array Error for hittime is not big enough" << endl;
    
    
    tr->SetBranchAddress("pmtID", &id);
    tr->SetBranchAddress("hitTime", &hitTime);
    tr->GetEntry(1);
    
    ofstream pmtHitFile;
    pmtHitFile.open("eve_data/PMT_hits.txt");
    
    for (Int_t j = 0; j < eveNum; j++) {
        tr->GetEntry(j);
        cout << j << " " << "the total photon number is: " << nPhotons << endl;
        pmtHitFile <<"Event "<<j<<endl;
        
        vector<HitData> iHitData;
        
        for (Int_t i = 0; i < nPhotons; i++) {
            HitData tmpHitData;
            tmpHitData.event = j;
            tmpHitData.pmtID = id[i];
            tmpHitData.hitTime = hitTime[i];
            tmpHitData.hitNum = 0;
            
            iHitData.push_back(tmpHitData);
        }
        sort(iHitData.begin(), iHitData.end()); //sort the raw data, so we can  store the first time when covering
        
        vector<HitData> pmtSortHitData;
        
        for (Int_t i = 0; i <= 70000; i++) {    //40000 here ought to be the max pmt number; new an ID-based queue
            HitData tmpHitData;
            pmtSortHitData.push_back(tmpHitData);
        }
        
        
        for (Int_t i = 0; i < nPhotons; i++) {
            //cover the new queue with the hit data
            //            cout << i << " " << iHitData[i].pmtID << endl;
            int id_index = iHitData[i].pmtID;
            if (iHitData[i].pmtID > 29999 && iHitData[i].pmtID < 40000) {
                //for veto pmts
                id_index = iHitData[i].pmtID - 10000;
            }
            if (iHitData[i].pmtID > 299999) {
                //for 3inch pmt, copyNo>300000
                id_index = iHitData[i].pmtID - 270000;
            }
            
            if (pmtSortHitData[id_index].hitNum == -1) {
                pmtSortHitData[id_index] = iHitData[i];
                pmtSortHitData[id_index].hitNum = 1;
            } else {
                pmtSortHitData[id_index].hitNum = pmtSortHitData[id_index].hitNum + 1;    //count the hit
            }
        }
        
        
        for (vector<HitData>::iterator it = pmtSortHitData.begin(); it != pmtSortHitData.end();) {
            if ((*it).hitNum == -1) {
                it = pmtSortHitData.erase(it);//remove the pmt which is not hit
            } else {
                it++;
            }
        }
        
        //        sort(pmtSortHitData.begin(), pmtSortHitData.end());
        
        for (vector<HitData>::iterator it = pmtSortHitData.begin(); it != pmtSortHitData.end(); it++) {
            pmtHitFile << (*it).pmtID << " " << (*it).hitTime << " " << (*it).hitNum << endl;
            //pmtHitFile << j << " " << (*it).pmtID << " " << (*it).hitTime << " " << (*it).hitNum << endl;
            
        }
        
        cout << "Finished " << j << "/" << eveNum << endl;
        
    }
    
    
    
    cout << ("This is the end (゜-゜)つロ ") << endl;
    
    pmtHitFile.close();
    
    gROOT->ProcessLine(".q");
    
    
    return 0;
}



