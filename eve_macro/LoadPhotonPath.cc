// Load the optical photon tracks.
// Created by zhuj on 2018/4/24.
//

#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TGeoNode.h>
#include <TVector3.h>
#include <TMath.h>
#include <TGeoPhysicalNode.h>
#include <fstream>
#include <iostream>
#include <iomanip>


using namespace std;

int rate = 100;  //the ratio to extract the tracks. 1/rate * 100%


void LoadPhotonPath() {
    TFile *f = TFile::Open("../sample_detsim_user.root");
    gDirectory->ls();
    if(f==NULL) {
        cout<<"Can not sample_detsim_user.root in ../"<<endl;
        gROOT->ProcessLine(".q");
    }
    TTree *tr = (TTree *) (gDirectory->Get("opsteps"));
    if(tr==NULL) {
        cout<<"Can not find TTree opsteps"<<endl;
        cout<<"Check the README.txt to learn about how to output the OP tracks in offline"<<endl;
        gROOT->ProcessLine(".q");
    }
    
    tr->Print();
    
    ofstream opFile;
    opFile.open("eve_data/photon_path.txt");
    
    Long64_t eveNum;
    eveNum = tr->GetEntries();
    if(eveNum>100) eveNum =100; //the limit size
    
    
    
    
    vector<int> *trkID;
    vector<int> *ptrkID;
    vector<float> *preT;
    vector<float> *preX;
    vector<float> *preY;
    vector<float> *preZ;
    vector<float> *postT;
    vector<float> *postX;
    vector<float> *postY;
    vector<float> *postZ;
    vector<int> *opType;
    
    for (int j = 0; j < eveNum; j++) {
        //        tr->SetMakeClass(1);
        
        tr->SetBranchAddress("TrackID", &trkID);
        tr->SetBranchAddress("ParentID", &ptrkID);
        tr->SetBranchAddress("PreT", &preT);
        tr->SetBranchAddress("PreX", &preX);
        tr->SetBranchAddress("PreY", &preY);
        tr->SetBranchAddress("PreZ", &preZ);
        tr->SetBranchAddress("PostT", &postT);
        tr->SetBranchAddress("PostX", &postX);
        tr->SetBranchAddress("PostY", &postY);
        tr->SetBranchAddress("PostZ", &postZ);
        tr->SetBranchAddress("OPType", &opType);
        
        
        tr->GetEntry(j);
        
        cout << j << " number of trackID " << trkID->size() << endl;
        cout << j << " number of parent_trackID " << ptrkID->size() << endl;
        cout << j << " number of PreT " << preT->size() << endl;
        cout << j << " number of PreX " << preX->size() << endl;
        cout << j << " number of PreY " << preY->size() << endl;
        cout << j << " number of PreZ " << preZ->size() << endl;
        cout << j << " number of PostT " << postT->size() << endl;
        cout << j << " number of PostX " << postX->size() << endl;
        cout << j << " number of PostY " << postY->size() << endl;
        cout << j << " number of PostZ " << postZ->size() << endl;
        cout << j << " number of OPType " << opType->size() << endl;
        
        
        opFile<<"Event "<<j<<endl;
        
        vector <int> trackNum;
        int lastTrackID = -1;
        
        vector<int> tID, pID, type;
        vector<float> prT, prX, prY, prZ, poT, poX, poY, poZ;
        tID = *trkID;
        pID = *ptrkID;
        prT = *preT;
        prX = *preX;
        prY = *preY;
        prZ = *preZ;
        poT = *postT;
        poX = *postX;
        poY = *postY;
        poZ = *postZ;
        type = *opType;
        
        for (int i = 0; i<trkID->size(); i++) {
            
            if (i == 0) {
                trackNum.push_back(tID[i]);
            }
            
            if (lastTrackID != tID[i] && i != 0) {
                //count the amount of track id
                trackNum.push_back(tID[i]);
            }
            
            if(trackNum.size()%rate==1) {
                if(trackNum.size()%(rate)==1) {
                    cout<<"Outputing track "<<trackNum.size()<<"  interval "<<i<<"/"<<trkID->size()<<endl;
                }
                
                opFile << i << " " << tID[i] << " " << pID[i] << " "
                << prT[i] << " " << prX[i] << " " << prY[i] << " " << prZ[i] << " "
                << poT[i] << " " << poX[i] << " " << poY[i] << " " << poZ[i] << " " << type[i] << endl;
                
            }
            lastTrackID = tID[i];
        }
        
    }
    
    opFile.close();
    
    gROOT->ProcessLine(".q");
    
}

