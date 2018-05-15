#Guide to use the event display macro
#Created by zhuj

#Quick Start
#First, sample_detsim_user.root, sample_rec.root and eve_macro/ should in the same directory.
sh eve_macro.sh
cp -r eve_data <your event display executable program directory>


#About the script
eve_macro.sh is a bash script to run LoadVertexes.cc, LoadPhotonPath.cc and LoadSortedPMTHit.cc with ROOT compiler.

All the data will be saved in the ./eve_data
Usually there are five file, sim_position.txt, rec_position.txt, edep_position.txt, photon_path.txt, PMT_hits.txt.
Copy the ./eve_data to the directory of the event display system for your operating system.
The eve_data folder should in the same level with the executable file, like .app or .exe.


#MORE
LoadVertexes.cc is to read out the vertexes positions from sample_detsim_user.root and sample_rec.root

LoadPhotonPath.cc is to output the optical photon path from sample_detsim_user.root.
You can find the "opsteps" tree in the root file if you add  "--anamgr-photon-tracking" when running the tut_detsim.py.

LoadSortedPMTHit.cc is to output the first hit time and P.E. number in each PMT.

Only the first 100 events will be loaded.



