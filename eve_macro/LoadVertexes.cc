// Load the initial, deposited and reconstructed vertexes.
// Created by zhuj on 2018/4/24.
//

#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>

#include <fstream>
#include <vector>
#include <iostream>

using namespace std;


void LoadSim();

void LoadDep();

void LoadRec();

void LoadVertexes(){
    
    LoadSim();
    
    LoadDep();
    
    LoadRec();
    
    gROOT->ProcessLine(".q");
    
}


void LoadSim() {
    //gROOT->ProcessLine(".L $SIMEVENTV2ROOT/$CMTCONFIG/libSimEventV2.so");
    TFile *f = TFile::Open("../sample_detsim_user.root");
    if(f==NULL) {
        cout<<"Can not sample_detsim_user.root in ../"<<endl;
    } else {
        
        gDirectory->ls();
        TTree *tr = (TTree *) (gDirectory->Get("geninfo"));
        //tr->Print();
        
        ofstream simfile;
        simfile.open("eve_data/sim_position.txt");
        
        Long64_t eveNum;
        eveNum = tr->GetEntries();
        if (eveNum > 100) eveNum = 100; //the limit size
        
        for (int i = 0; i < eveNum; i++) {
            
            const int np = 3;
            
            static Float_t m_x[10], m_y[10], m_z[10], p_x[10], p_y[10], p_z[10];
            
            tr->SetBranchAddress("InitX", &m_x);
            tr->SetBranchAddress("InitY", &m_y);
            tr->SetBranchAddress("InitZ", &m_z);
            
            tr->SetBranchAddress("InitPX", &p_x);
            tr->SetBranchAddress("InitPY", &p_y);
            tr->SetBranchAddress("InitPZ", &p_z);
            
            tr->GetEntry(i);
            
            //to simplify, we only output the m_x[0]
            simfile << m_x[0] << " " << m_y[0] << " " << m_z[0] << " "
            << p_x[0] << " " << p_y[0] << " " << p_z[0] << endl;
            
        }
        simfile.close();
    }
}

void LoadDep() {
    TFile *f = TFile::Open("../sample_detsim_user.root");
    gDirectory->ls();
    TTree *tr = (TTree *) (gDirectory->Get("evt"));
    //tr->Print();
    if(f==NULL) {
        cout<<"Can not sample_detsim_user.root in ../"<<endl;
    } else {
        
        ofstream depfile;
        
        depfile.open("eve_data/edep_position.txt");
        Float_t edep_x, edep_y, edep_z, edep;
        
        Long64_t eveNum;
        eveNum = tr->GetEntries();
        if (eveNum > 100) eveNum = 100; //the limit size
        
        for (int i = 0; i < eveNum; i++) {
            tr->SetBranchAddress("edepX", &edep_x);
            tr->SetBranchAddress("edepY", &edep_y);
            tr->SetBranchAddress("edepZ", &edep_z);
            tr->SetBranchAddress("edep", &edep);
            
            tr->GetEntry(i);
            
            depfile << edep_x << " " << edep_y << " " << edep_z << " " << edep << endl;
            
        }
        depfile.close();
        
    }
}

void LoadRec() {
    TFile *f = TFile::Open("../sample_rec.root");
    if(f==NULL) {
        cout<<"Can not sample_rec.root in ../"<<endl;
        gROOT->ProcessLine(".q");
    }
    
    gDirectory->cd("Event");
    gDirectory->cd("Rec;1");            //the new structure in J17v1r1
    TTree *tr = (TTree *) (gDirectory->Get("CDRecEvent;1"));       //the new structure in J17v1r1
    //tr->Print();
    
    ofstream recfile;
    recfile.open("eve_data/rec_position.txt");
    
    vector<Double_t> m_x, m_y, m_z, m_energy;
    UInt_t m_nVertexes = 0;
    
    Long64_t eveNum;
    eveNum = tr->GetEntries();
    if(eveNum>100) eveNum =100; //the limit size
    
    for (int i = 0; i < eveNum; i++) {
        tr->SetMakeClass(1);
        tr->SetBranchAddress("m_nVertexes", &m_nVertexes);
        tr->SetBranchAddress("m_x", &m_x);
        tr->SetBranchAddress("m_y", &m_y);
        tr->SetBranchAddress("m_z", &m_z);
        tr->SetBranchAddress("m_eprec", &m_energy);
        tr->GetEntry(i);
        
        recfile << m_x[0] << " " << m_y[0] << " " << m_z[0] << " " << m_energy[0] << endl;
        
    }
    recfile.close();
    
}

