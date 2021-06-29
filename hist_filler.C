#include "hist_filler.h"

int main(int argc, char* argv[]){

  TString channel;
  TString year = "FullRun2";
  TString yearlabel;


  if(argc < 3){
    cout << "[ERROR] you have to specify the channel (muon/elec)!" << endl;
    cout << "./histfiller <channel> <year>" << endl;

    return 0;
  }

  if(strcmp(argv[1], "muon") == 0)              channel = "muon";
  else if(strcmp(argv[1], "elec") == 0)         channel = "elec";
  else if(strcmp(argv[1], "muon_elecJMS") == 0) channel = "muon_elecJMS";
  else if(strcmp(argv[1], "elec_muonJMS") == 0) channel = "elec_muonJMS";
  else{
    cout << "[ERROR] Channel not known, select muon or elec!" << endl;
    return 0;
  }

  if(argc > 2){
    if(strcmp(argv[2], "2016") == 0){
      year = "2016";
      yearlabel = "2016v3";
    }
    else if(strcmp(argv[2], "2017") == 0){
      year = "2017";
      yearlabel = "2017v2";
    }
    else if(strcmp(argv[2], "2018") == 0){
      year = "2018";
      yearlabel = "2018";
    }
  }

  ttweight = false;

  if(ttweight) cout << "ttbar reweighting is applied" << endl;
  else         cout << "ttbar reweighting is NOT applied" << endl;

  // switch on histogram errors
  TH1::SetDefaultSumw2();

  /*
   ██████ ██████  ███████  █████  ████████ ███████      ██████  ██    ██ ████████ ██████  ██    ██ ████████     ███████ ██ ██      ███████
  ██      ██   ██ ██      ██   ██    ██    ██          ██    ██ ██    ██    ██    ██   ██ ██    ██    ██        ██      ██ ██      ██
  ██      ██████  █████   ███████    ██    █████       ██    ██ ██    ██    ██    ██████  ██    ██    ██        █████   ██ ██      █████
  ██      ██   ██ ██      ██   ██    ██    ██          ██    ██ ██    ██    ██    ██      ██    ██    ██        ██      ██ ██      ██
   ██████ ██   ██ ███████ ██   ██    ██    ███████      ██████   ██████     ██    ██       ██████     ██        ██      ██ ███████ ███████
  */

  TString filename = "Histograms_"+year+"_"+channel+".root";

  outputFile=new TFile(filename,"recreate");
  outputFile->cd();

  /*
  ██████  ███████  █████  ██████      ██████  ██ ███    ██ ███    ██ ██ ███    ██  ██████
  ██   ██ ██      ██   ██ ██   ██     ██   ██ ██ ████   ██ ████   ██ ██ ████   ██ ██
  ██████  █████   ███████ ██   ██     ██████  ██ ██ ██  ██ ██ ██  ██ ██ ██ ██  ██ ██   ███
  ██   ██ ██      ██   ██ ██   ██     ██   ██ ██ ██  ██ ██ ██  ██ ██ ██ ██  ██ ██ ██    ██
  ██   ██ ███████ ██   ██ ██████      ██████  ██ ██   ████ ██   ████ ██ ██   ████  ██████
  */

  // read binning schemes in XML format
  TDOMParser parser;
  TString binning_xml = "Binning.xml";

  Int_t error=parser.ParseFile(binning_xml);
  if(error) cout<<"error="<<error<<" from TDOMParser\n";
  TXMLDocument const *XMLdocument=parser.GetXMLDocument();
  binning_rec = TUnfoldBinningXML::ImportXML(XMLdocument,"binning_rec");
  binning_gen = TUnfoldBinningXML::ImportXML(XMLdocument,"binning_gen");
  binning_gen20 = TUnfoldBinningXML::ImportXML(XMLdocument,"binning_gen20");


  if(!binning_rec) cout<<"could not read 'rec' binning\n";
  if(!binning_gen) cout<<"could not read 'gen' binning\n";
  if(!binning_gen20) cout<<"could not read 'gen20' binning\n";


  // get distributions from measurement phase space and sideband regions
  measurement_rec = binning_rec->FindNode("measurement_rec");
  measurement_gen = binning_gen->FindNode("measurement_gen");
  measurement_gen20 = binning_gen20->FindNode("measurement_gen20");

  ptmigration_rec = binning_rec->FindNode("ptmigration_rec");
  ptmigration_gen = binning_gen->FindNode("ptmigration_gen");

  subptmigration_rec = binning_rec->FindNode("subptmigration_rec");
  subptmigration_gen = binning_gen->FindNode("subptmigration_gen");

  massmigration_rec = binning_rec->FindNode("massmigration_rec");
  massmigration_gen = binning_gen->FindNode("massmigration_gen");

  leptonptmigration_rec = binning_rec->FindNode("leptonptmigration_rec");
  leptonptmigration_gen = binning_gen->FindNode("leptonptmigration_gen");

  btagmigration_rec = binning_rec->FindNode("btagmigration_rec");

  /*
  ███████ ██ ██      ██          ██   ██ ██ ███████ ████████  ██████   ██████  ██████   █████  ███    ███ ███████
  ██      ██ ██      ██          ██   ██ ██ ██         ██    ██    ██ ██       ██   ██ ██   ██ ████  ████ ██
  █████   ██ ██      ██          ███████ ██ ███████    ██    ██    ██ ██   ███ ██████  ███████ ██ ████ ██ ███████
  ██      ██ ██      ██          ██   ██ ██      ██    ██    ██    ██ ██    ██ ██   ██ ██   ██ ██  ██  ██      ██
  ██      ██ ███████ ███████     ██   ██ ██ ███████    ██     ██████   ██████  ██   ██ ██   ██ ██      ██ ███████
  */

  cout << "Year = " << year << endl;


  // define directory
  TString dir = "/nfs/dust/cms/user/schwarzd/MTopJet_Run2/PostSel/"+channel+"/";
  TString prefix = "/uhh2.AnalysisModuleRunner.";

  // fill data
  TFile *data_File=new TFile(dir+prefix+"DATA.DATA_"+yearlabel+".root");
  fill_data((TTree *) data_File->Get("AnalysisTree"));
  // fill ttbar
  TFile *mc_matrix_File=new TFile(dir+prefix+"MC.TTbar_"+yearlabel+".root");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "mc", "none");
  // fill TTbar_amcatnlo
  // TFile *pseudodata_File=new TFile(dir+prefix+"MC.TTbar_amcatnlo-pythia.root");
  TFile *pseudodata_File=new TFile(dir+prefix+"MC.TTbar_mtop1715_"+yearlabel+".root");
  fill_matrix((TTree *) pseudodata_File->Get("AnalysisTree"), "pseudo1", "none");
  //fill TTbar_powheg
  // TFile *pseudodata2_File=new TFile(dir+prefix+"MC.TTbar_powheg-herwig.root");
  TFile *pseudodata2_File=new TFile(dir+prefix+"MC.TTbar_mtop1735_"+yearlabel+".root");
  fill_matrix((TTree *) pseudodata2_File->Get("AnalysisTree"), "pseudo2", "none");

  // fill SYS (weights)
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "muidup", "sf_muid_up");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "muiddown", "sf_muid_down");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "mutrup", "sf_mutr_up");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "mutrdown", "sf_mutr_down");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "elidup", "sf_elid_up");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "eliddown", "sf_elid_down");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "eltrup", "sf_eltr_up");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "eltrdown", "sf_eltr_down");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "elrecoup", "sf_elreco_up");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "elrecodown", "sf_elreco_down");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "puup", "sf_pu_up");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "pudown", "sf_pu_down");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "btagup", "sf_btag_up");
  fill_matrix((TTree *) mc_matrix_File->Get("AnalysisTree"), "btagdown", "sf_btag_down");

  // fill shape SYS
  vector<TString> sys_name;
  vector<TString> subdir;
  sys_name.push_back("jecup");
  sys_name.push_back("jecdown");
  sys_name.push_back("jerup");
  sys_name.push_back("jerdown");
  sys_name.push_back("corup");
  sys_name.push_back("cordown");
  sys_name.push_back("jmsupup");
  sys_name.push_back("jmsupdown");
  sys_name.push_back("jmsdownup");
  sys_name.push_back("jmsdowndown");

  subdir.push_back("JEC_up");
  subdir.push_back("JEC_down");
  subdir.push_back("JER_up");
  subdir.push_back("JER_down");
  subdir.push_back("COR_up");
  subdir.push_back("COR_down");
  subdir.push_back("JMS_upup");
  subdir.push_back("JMS_updown");
  subdir.push_back("JMS_downup");
  subdir.push_back("JMS_downdown");

  for(unsigned int i=0; i<sys_name.size(); i++){
    TFile *file = new TFile(dir+subdir[i]+prefix+"MC.TTbar_"+yearlabel+".root");
    fill_matrix((TTree *) file->Get("AnalysisTree"), sys_name[i], "none");
    delete file;
  }



  // fill model SYS (weights)
  fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SCALE_upup", "sf_scale_upup");
  fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SCALE_upnone", "sf_scale_upnone");
  fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SCALE_noneup", "sf_scale_noneup");
  fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SCALE_nonedown", "sf_scale_nonedown");
  fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SCALE_downnone", "sf_scale_downnone");
  fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SCALE_downdown", "sf_scale_downdown");

  if(year != "2016"){
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_fsrup_sqrt2", "sf_fsr_upsqrt2");
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_fsrup_2", "sf_fsr_up2");
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_fsrup_4", "sf_fsr_up4");
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_fsrdown_sqrt2", "sf_fsr_downsqrt2");
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_fsrdown_2", "sf_fsr_down2");
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_fsrdown_4", "sf_fsr_down4");
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_isrup_2", "sf_isr_up2");
    fill_modelsys((TTree *) mc_matrix_File->Get("AnalysisTree"), "SHOWER_isrdown_2", "sf_isr_down2");
  }

  // fill model SYS (files)
  TFile * file_tuneup = new TFile(dir+prefix+"MC.TTbar_TuneUp_"+yearlabel+".root");
  TFile * file_tunedown = new TFile(dir+prefix+"MC.TTbar_TuneDown_"+yearlabel+".root");
  TFile * file_hdampup = new TFile(dir+prefix+"MC.TTbar_hdampup_"+yearlabel+".root");
  TFile * file_hdampdown = new TFile(dir+prefix+"MC.TTbar_hdampdown_"+yearlabel+".root");
  TFile * file_gluonmove = new TFile(dir+prefix+"MC.TTbar_GluonMove_"+yearlabel+".root");
  TFile * file_qcdbased = new TFile(dir+prefix+"MC.TTbar_QCDbased_"+yearlabel+".root");

  fill_modelsys((TTree *) file_hdampup->Get("AnalysisTree"), "SHOWER_hdampup", "none");
  fill_modelsys((TTree *) file_hdampdown->Get("AnalysisTree"), "SHOWER_hdampdown", "none");
  fill_modelsys((TTree *) file_tuneup->Get("AnalysisTree"), "TUNE_up", "none");
  fill_modelsys((TTree *) file_tunedown->Get("AnalysisTree"), "TUNE_down", "none");
  fill_modelsys((TTree *) file_gluonmove->Get("AnalysisTree"), "CR_gluonmove", "none");
  fill_modelsys((TTree *) file_qcdbased->Get("AnalysisTree"), "CR_qcdbased", "none");

  if(year == "2016"){
    TFile * file_fsrup = new TFile(dir+prefix+"MC.TTbar_fsrup_2016v3.root");
    TFile * file_fsrdown = new TFile(dir+prefix+"MC.TTbar_fsrdown_2016v3.root");
    TFile * file_isrup = new TFile(dir+prefix+"MC.TTbar_isrup_2016v3.root");
    TFile * file_isrdown = new TFile(dir+prefix+"MC.TTbar_isrdown_2016v3.root");

    fill_modelsys((TTree *) file_fsrup->Get("AnalysisTree"), "SHOWER_fsrup_2", "none");
    fill_modelsys((TTree *) file_fsrdown->Get("AnalysisTree"), "SHOWER_fsrdown_2", "none");
    fill_modelsys((TTree *) file_isrup->Get("AnalysisTree"), "SHOWER_isrup_2", "none");
    fill_modelsys((TTree *) file_isrdown->Get("AnalysisTree"), "SHOWER_isrdown_2", "none");
  }

  // fill_breweight( (TTree *) mc_matrix_File->Get("AnalysisTree"), "breweight_up");
  // fill_breweight( (TTree *) mc_matrix_File->Get("AnalysisTree"), "breweight_down");


  // fill PDF
  // fill_pdf( (TTree *) mc_matrix_File->Get("AnalysisTree") );

  // fill mass templates
  std::vector<TFile *> mc_truth_File;
  mc_truth_File.push_back(new TFile(dir+prefix+"MC.TTbar_mtop1665_"+yearlabel+".root"));
  mc_truth_File.push_back(new TFile(dir+prefix+"MC.TTbar_mtop1695_"+yearlabel+".root"));
  mc_truth_File.push_back(new TFile(dir+prefix+"MC.TTbar_mtop1715_"+yearlabel+".root"));
  mc_truth_File.push_back(new TFile(dir+prefix+"MC.TTbar_mtop1735_"+yearlabel+".root"));
  mc_truth_File.push_back(new TFile(dir+prefix+"MC.TTbar_mtop1755_"+yearlabel+".root"));
  mc_truth_File.push_back(new TFile(dir+prefix+"MC.TTbar_mtop1785_"+yearlabel+".root"));

  fill_template((TTree *) mc_truth_File[0]->Get("AnalysisTree"), "1665");
  fill_template((TTree *) mc_truth_File[1]->Get("AnalysisTree"), "1695");
  fill_template((TTree *) mc_truth_File[2]->Get("AnalysisTree"), "1715");
  fill_template((TTree *) mc_truth_File[3]->Get("AnalysisTree"), "1735");
  fill_template((TTree *) mc_truth_File[4]->Get("AnalysisTree"), "1755");
  fill_template((TTree *) mc_truth_File[5]->Get("AnalysisTree"), "1785");

  fill_additionaltemplates(channel, year);

  // also fill mass samples as pseudo
  fill_matrix((TTree *) mc_truth_File[1]->Get("AnalysisTree"), "pseudo1695", "none");
  fill_matrix((TTree *) mc_truth_File[2]->Get("AnalysisTree"), "pseudo1715", "none");
  fill_matrix((TTree *) mc_truth_File[3]->Get("AnalysisTree"), "pseudo1735", "none");
  fill_matrix((TTree *) mc_truth_File[4]->Get("AnalysisTree"), "pseudo1755", "none");

  // fill background
  vector<TString> bkg_name = {"WJets", "SingleTop", "other"};
  for(unsigned int i=0; i<bkg_name.size(); i++){
    TFile *file = new TFile(dir+prefix+"MC."+bkg_name[i]+"_"+yearlabel+".root");
    fill_background((TTree *) file->Get("AnalysisTree"), bkg_name[i]);
    delete file;
  }

  outputFile->Close();
  return 0;
}


/*
███████ ██ ██      ██          ██████   █████  ████████  █████
██      ██ ██      ██          ██   ██ ██   ██    ██    ██   ██
█████   ██ ██      ██          ██   ██ ███████    ██    ███████
██      ██ ██      ██          ██   ██ ██   ██    ██    ██   ██
██      ██ ███████ ███████     ██████  ██   ██    ██    ██   ██
*/



void fill_data(TTree* tree){
  if(!tree) cout << "could not read 'data' tree\n";
  else      cout << "Filling Data Histograms...\n";

  TH1* h_data = binning_rec->CreateHistogram("data");

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_subptmigration_rec",&passed_subptmigration_rec);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_leptonptmigration_rec",&passed_leptonptmigration_rec);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);

  tree->SetBranchStatus("*",1);

  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;

    Int_t binNumber = 0;
    if     (passed_measurement_rec)    binNumber = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    binNumber = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_subptmigration_rec) binNumber = subptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_massmigration_rec)  binNumber = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_leptonptmigration_rec)binNumber = leptonptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  binNumber = btagmigration_rec->GetGlobalBinNumber(massRec);


    if(passed_measurement_rec || passed_ptmigration_rec || passed_subptmigration_rec || passed_massmigration_rec ||passed_leptonptmigration_rec || passed_btagmigration_rec){
      h_data->Fill(binNumber);
    }
  }

  h_data->Write();
  delete h_data;
  return;
}

/*
███████ ██ ██      ██          ███    ███  █████  ███████ ███████     ████████ ███████ ███    ███ ██████
██      ██ ██      ██          ████  ████ ██   ██ ██      ██             ██    ██      ████  ████ ██   ██
█████   ██ ██      ██          ██ ████ ██ ███████ ███████ ███████        ██    █████   ██ ████ ██ ██████
██      ██ ██      ██          ██  ██  ██ ██   ██      ██      ██        ██    ██      ██  ██  ██ ██
██      ██ ███████ ███████     ██      ██ ██   ██ ███████ ███████        ██    ███████ ██      ██ ██
*/

void fill_template(TTree* tree, TString mtop){
  if(!tree) cout << "could not read 'data' tree\n";
  else      cout << "Filling Template Histograms...\n";

  // TH1* h_mass_truth = measurement_gen->CreateHistogram("mc_mtop"+mtop+"_truth",kTRUE,0,0,axissteer);
  TH1* h_mass_truth = measurement_gen20->CreateHistogram("mc_mtop"+mtop+"_truth",kTRUE,0,0,axissteer);
  TH1* h_mass_sig = binning_rec->CreateHistogram("mc_mtop"+mtop+"_sig");

  outputFile->cd();
  tree->ResetBranchAddresses();
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Mass_Gen33",&massGen);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("Pt_Gen33",&ptGen);
  // tree->SetBranchAddress("matched",&merged);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("passed_measurement_gen",&passed_measurement_gen);
  tree->SetBranchAddress("is_TTbar",&is_TTbar);
  tree->SetBranchAddress("rec_weight",&rec_weight);
  tree->SetBranchAddress("gen_weight",&gen_weight);
  tree->SetBranchAddress("gen_weight_ttfactor",&gen_ttfactor);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_ptmigration_gen",&passed_ptmigration_gen);
  tree->SetBranchAddress("passed_subptmigration_rec",&passed_subptmigration_rec);
  tree->SetBranchAddress("passed_subptmigration_gen",&passed_subptmigration_gen);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_massmigration_gen",&passed_massmigration_gen);
  tree->SetBranchAddress("passed_leptonptmigration_rec",&passed_leptonptmigration_rec);
  tree->SetBranchAddress("passed_leptonptmigration_gen",&passed_leptonptmigration_gen);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);
  tree->SetBranchStatus("*",1);


  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;
    if(!is_TTbar){
      cout << "ERROR: non-ttbar event in signal sample!" << endl;
      break;
    }

    if(ttweight) gen_weight *= gen_ttfactor;

    // get weight for gen and rec hists
    w_sig_rec = rec_weight * gen_weight;
    w_gen = gen_weight;


    Int_t recBin;
    if     (passed_measurement_rec)    recBin = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    recBin = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_subptmigration_rec) recBin = subptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_massmigration_rec)  recBin = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_leptonptmigration_rec) recBin = leptonptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  recBin = btagmigration_rec->GetGlobalBinNumber(massRec);
    else                               recBin = 0;

    bool rec_info = false;
    if(passed_ptmigration_rec || passed_measurement_rec || passed_massmigration_rec || passed_subptmigration_rec || passed_leptonptmigration_rec ||passed_btagmigration_rec) rec_info = true;


    // Fill 1D Hists (for truth distribution only use gen selection)
    if(passed_measurement_gen) h_mass_truth->Fill(massGen, w_gen);
    if(rec_info) h_mass_sig->Fill(recBin, w_sig_rec);

  }
  h_mass_sig->Write();
  h_mass_truth->Write();
  if(mtop != "1665" && mtop != "1785") h_mtop_truth.push_back(h_mass_truth);
  delete h_mass_sig;
  // delete h_mass_truth;
  return;
}

/*
███████ ██ ██      ██          ██████   █████   ██████ ██   ██  ██████  ██████   ██████  ██    ██ ███    ██ ██████
██      ██ ██      ██          ██   ██ ██   ██ ██      ██  ██  ██       ██   ██ ██    ██ ██    ██ ████   ██ ██   ██
█████   ██ ██      ██          ██████  ███████ ██      █████   ██   ███ ██████  ██    ██ ██    ██ ██ ██  ██ ██   ██
██      ██ ██      ██          ██   ██ ██   ██ ██      ██  ██  ██    ██ ██   ██ ██    ██ ██    ██ ██  ██ ██ ██   ██
██      ██ ███████ ███████     ██████  ██   ██  ██████ ██   ██  ██████  ██   ██  ██████   ██████  ██   ████ ██████
*/



void fill_background(TTree *tree, TString prefix){
  if(!tree) cout << "could not read 'mc bgr' tree\n";
  else      cout << "Filling Background Histograms...\n";

  TH1* h_mc_bgr = binning_rec->CreateHistogram("BKG_"+prefix);

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("rec_weight",&rec_weight);
  tree->SetBranchAddress("gen_weight",&gen_weight);
  tree->SetBranchAddress("gen_weight_ttfactor",&gen_ttfactor);
  tree->SetBranchAddress("is_TTbar",&is_TTbar);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_subptmigration_rec",&passed_subptmigration_rec);
  tree->SetBranchAddress("passed_leptonptmigration_rec",&passed_leptonptmigration_rec);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);
  tree->SetBranchStatus("*",1);



  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;
    if(is_TTbar){
      cout << "ERROR: ttbar event in background sample!" << endl;
      break;
    }

    if(ttweight) gen_weight *= gen_ttfactor;
    w_bgr_rec = rec_weight * gen_weight;

    Int_t recBin = 0;
    if     (passed_measurement_rec)    recBin = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    recBin = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_subptmigration_rec) recBin = subptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_massmigration_rec)  recBin = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_leptonptmigration_rec) recBin = leptonptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  recBin = btagmigration_rec->GetGlobalBinNumber(massRec);

    if(passed_measurement_rec || passed_ptmigration_rec || passed_subptmigration_rec || passed_massmigration_rec || passed_leptonptmigration_rec ||passed_btagmigration_rec){
      h_mc_bgr->Fill(recBin, w_bgr_rec);
    }
  }
  h_mc_bgr->Write();
  delete h_mc_bgr;
  return;
}


/*
███████ ██ ██      ██          ███    ███  █████  ████████ ██████  ██ ██   ██
██      ██ ██      ██          ████  ████ ██   ██    ██    ██   ██ ██  ██ ██
█████   ██ ██      ██          ██ ████ ██ ███████    ██    ██████  ██   ███
██      ██ ██      ██          ██  ██  ██ ██   ██    ██    ██   ██ ██  ██ ██
██      ██ ███████ ███████     ██      ██ ██   ██    ██    ██   ██ ██ ██   ██
*/

void fill_matrix(TTree* tree, TString prefix, TString weightname){
  if(!tree) cout<<"could not read 'mc signal' tree\n";
  else      cout << "Filling Matrix Histograms ("+prefix+") ...\n";

  // setup hists
  // TH1* h_purity_all = measurement_gen->CreateHistogram(prefix + "_purity_all",kTRUE,0,0,axissteer_purity);
  // TH1* h_purity_samebin = measurement_gen->CreateHistogram(prefix + "_purity_samebin",kTRUE,0,0,axissteer_purity);
  TH1* h_purity_all = measurement_gen20->CreateHistogram(prefix + "_purity_all",kTRUE,0,0,axissteer_purity);
  TH1* h_purity_samebin = measurement_gen20->CreateHistogram(prefix + "_purity_samebin",kTRUE,0,0,axissteer_purity);

  // TH1* h_stability_all = measurement_gen->CreateHistogram(prefix + "_stability_all",kTRUE,0,0,axissteer_purity);
  // TH1* h_stability_samebin = measurement_gen->CreateHistogram(prefix + "_stability_samebin",kTRUE,0,0,axissteer_purity);
  TH1* h_stability_all = measurement_gen20->CreateHistogram(prefix + "_stability_all",kTRUE,0,0,axissteer_purity);
  TH1* h_stability_samebin = measurement_gen20->CreateHistogram(prefix + "_stability_samebin",kTRUE,0,0,axissteer_purity);

  TH1* h_mc_sig = binning_rec->CreateHistogram(prefix + "_sig");
  TH1* h_mc_gen = binning_gen->CreateHistogram(prefix + "_gen");
  // TH1* h_mc_truth = measurement_gen->CreateHistogram(prefix + "_truth",kTRUE,0,0,axissteer);
  TH1* h_mc_truth = measurement_gen20->CreateHistogram(prefix + "_truth",kTRUE,0,0,axissteer);
  TH2* h_mc_matrix = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen,binning_rec, prefix + "_matrix");

  outputFile->cd();
  tree->ResetBranchAddresses();
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Mass_Gen33",&massGen);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("Pt_Gen33",&ptGen);
  // tree->SetBranchAddress("matched",&merged);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("passed_measurement_gen",&passed_measurement_gen);
  tree->SetBranchAddress("is_TTbar",&is_TTbar);
  tree->SetBranchAddress("rec_weight",&rec_weight);
  tree->SetBranchAddress("gen_weight",&gen_weight);
  tree->SetBranchAddress("gen_weight_ttfactor",&gen_ttfactor);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_ptmigration_gen",&passed_ptmigration_gen);
  tree->SetBranchAddress("passed_subptmigration_rec",&passed_subptmigration_rec);
  tree->SetBranchAddress("passed_subptmigration_gen",&passed_subptmigration_gen);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_massmigration_gen",&passed_massmigration_gen);
  tree->SetBranchAddress("passed_leptonptmigration_rec",&passed_leptonptmigration_rec);
  tree->SetBranchAddress("passed_leptonptmigration_gen",&passed_leptonptmigration_gen);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);
  if(weightname != "none"){
    cout << "  - using weight" << endl;
    tree->SetBranchAddress(weightname,&additional_factor);
  }
  tree->SetBranchStatus("*",1);


  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;
    if(!is_TTbar){
      cout << "ERROR: non-ttbar event in signal sample!" << endl;
      break;
    }

    if(ttweight) gen_weight *= gen_ttfactor;

    // Here, only rec weight is changed!
    if(weightname != "none") rec_weight *= additional_factor;


    // get weights for migration matrix
    w_central = rec_weight * gen_weight;
    w_nogen = rec_weight * gen_weight;
    w_norec = gen_weight;
    w_correction = gen_weight * (1 - rec_weight);

    // get weight for gen and rec hists
    w_sig_rec = rec_weight * gen_weight;
    w_gen = gen_weight;


    // get global bins
    Int_t genBin;
    if     (passed_measurement_gen)   genBin = measurement_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_ptmigration_gen)   genBin = ptmigration_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_subptmigration_gen)genBin = subptmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_massmigration_gen) genBin = massmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_leptonptmigration_gen)genBin = leptonptmigration_gen->GetGlobalBinNumber(massGen);
    else                              genBin = 0;

    Int_t recBin;
    if     (passed_measurement_rec)    recBin = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    recBin = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_subptmigration_rec) recBin = subptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_massmigration_rec)  recBin = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  recBin = btagmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_leptonptmigration_rec) recBin = leptonptmigration_rec->GetGlobalBinNumber(massRec);
    else                               recBin = 0;

    bool gen_info = false;
    bool rec_info = false;
    if(passed_ptmigration_gen || passed_measurement_gen || passed_massmigration_gen || passed_subptmigration_gen || passed_leptonptmigration_gen) gen_info = true;
    if(passed_ptmigration_rec || passed_measurement_rec || passed_massmigration_rec || passed_subptmigration_rec || passed_leptonptmigration_rec || passed_btagmigration_rec) rec_info = true;


    // Fill 1D Hists (for truth distribution only use gen selection)
    if(passed_measurement_gen) h_mc_truth->Fill(massGen, w_gen);

    if(gen_info) h_mc_gen->Fill(genBin, w_gen);
    if(rec_info){
      h_mc_sig->Fill(recBin, w_sig_rec);
    }

    // Fill Matrix
    if( rec_info &&  gen_info) h_mc_matrix->Fill(genBin, recBin, w_central);
    if( rec_info && !gen_info) h_mc_matrix->Fill(genBin, recBin, w_nogen);
    if(!rec_info &&  gen_info) h_mc_matrix->Fill(genBin, recBin, w_norec);
    if( rec_info &&  gen_info) h_mc_matrix->Fill(genBin,     0., w_correction); // this is needed because events that dont pass rec, have no rec_weight.

    //fill hists for purity
    int genBin_recInfo = 0;
    int genBin_genInfo = 0;
    if(passed_measurement_gen && passed_measurement_rec){
      h_stability_all->Fill(massRec, w_central);
      h_purity_all->Fill(massGen, w_central);
      // genBin_recInfo = measurement_gen->GetGlobalBinNumber(massRec,ptGen);
      // genBin_genInfo = measurement_gen->GetGlobalBinNumber(massGen,ptGen);
      genBin_recInfo = measurement_gen20->GetGlobalBinNumber(massRec,ptGen);
      genBin_genInfo = measurement_gen20->GetGlobalBinNumber(massGen,ptGen);
      if(genBin_recInfo == genBin_genInfo){
        h_stability_samebin->Fill(massRec, w_central);
        h_purity_samebin->Fill(massGen, w_central);
      }
    }


  }
  h_purity_all->Write();
  h_purity_samebin->Write();
  h_stability_all->Write();
  h_stability_samebin->Write();
  h_mc_sig->Write();
  h_mc_gen->Write();
  h_mc_truth->Write();
  h_mc_matrix->Write();

  delete h_purity_all;
  delete h_purity_samebin;
  delete h_stability_all;
  delete h_stability_samebin;
  delete h_mc_sig;
  delete h_mc_gen;
  delete h_mc_truth;
  delete h_mc_matrix;

  return;
}

/*
███████ ██ ██      ██          ███    ███  ██████  ██████  ███████ ██
██      ██ ██      ██          ████  ████ ██    ██ ██   ██ ██      ██
█████   ██ ██      ██          ██ ████ ██ ██    ██ ██   ██ █████   ██
██      ██ ██      ██          ██  ██  ██ ██    ██ ██   ██ ██      ██
██      ██ ███████ ███████     ██      ██  ██████  ██████  ███████ ███████
*/

void fill_modelsys(TTree* tree, TString prefix, TString weightname){
  if(!tree) cout<<"could not read 'mc signal' tree\n";
  else      cout << "Filling Model SYS Histograms ("+prefix+") ...\n";

  // setup hists
  TH1* h_mc_sig = binning_rec->CreateHistogram(prefix + "_sig");
  TH1* h_mc_gen = binning_gen->CreateHistogram(prefix + "_gen");
  // TH1* h_mc_truth = measurement_gen->CreateHistogram(prefix + "_truth",kTRUE,0,0,axissteer);
  TH1* h_mc_truth = measurement_gen20->CreateHistogram(prefix + "_truth",kTRUE,0,0,axissteer);

  outputFile->cd();
  tree->ResetBranchAddresses();
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Mass_Gen33",&massGen);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("Pt_Gen33",&ptGen);
  // tree->SetBranchAddress("matched",&merged);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("passed_measurement_gen",&passed_measurement_gen);
  tree->SetBranchAddress("is_TTbar",&is_TTbar);
  tree->SetBranchAddress("rec_weight",&rec_weight);
  tree->SetBranchAddress("gen_weight",&gen_weight);
  tree->SetBranchAddress("gen_weight_ttfactor",&gen_ttfactor);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_ptmigration_gen",&passed_ptmigration_gen);
  tree->SetBranchAddress("passed_subptmigration_rec",&passed_subptmigration_rec);
  tree->SetBranchAddress("passed_subptmigration_gen",&passed_subptmigration_gen);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_massmigration_gen",&passed_massmigration_gen);
  tree->SetBranchAddress("passed_leptonptmigration_rec",&passed_leptonptmigration_rec);
  tree->SetBranchAddress("passed_leptonptmigration_gen",&passed_leptonptmigration_gen);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);
  if(weightname != "none"){
    cout << "  - using weight" << endl;
    tree->SetBranchAddress(weightname,&additional_factor);
  }
  tree->SetBranchStatus("*",1);


  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;
    if(!is_TTbar){
      cout << "ERROR: non-ttbar event in signal sample!" << endl;
      break;
    }

    if(ttweight) gen_weight *= gen_ttfactor;
    // Change gen weight here
    if(weightname != "none") gen_weight *= additional_factor;

    // get weight for gen and rec hists
    w_sig_rec = rec_weight * gen_weight;
    w_gen = gen_weight;

    // get global bins
    Int_t genBin;
    if     (passed_measurement_gen)   genBin = measurement_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_ptmigration_gen)   genBin = ptmigration_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_subptmigration_gen)genBin = subptmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_massmigration_gen) genBin = massmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_leptonptmigration_gen)genBin = leptonptmigration_gen->GetGlobalBinNumber(massGen);
    else                              genBin = 0;

    Int_t recBin;
    if     (passed_measurement_rec)    recBin = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    recBin = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_subptmigration_rec) recBin = subptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_massmigration_rec)  recBin = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_leptonptmigration_rec) recBin = leptonptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  recBin = btagmigration_rec->GetGlobalBinNumber(massRec);
    else                               recBin = 0;

    bool gen_info = false;
    bool rec_info = false;
    if(passed_ptmigration_gen || passed_measurement_gen || passed_massmigration_gen || passed_subptmigration_gen || passed_leptonptmigration_gen) gen_info = true;
    if(passed_ptmigration_rec || passed_measurement_rec || passed_massmigration_rec || passed_subptmigration_rec || passed_leptonptmigration_rec || passed_btagmigration_rec) rec_info = true;


    // Fill 1D Hists (for truth distribution only use gen selection)
    if(passed_measurement_gen) h_mc_truth->Fill(massGen, w_gen);
    if(rec_info) h_mc_sig->Fill(recBin, w_sig_rec);
    if(gen_info) h_mc_gen->Fill(genBin, w_gen);


  }

  h_mc_sig->Write();
  h_mc_truth->Write();
  h_mc_gen->Write();

  delete h_mc_gen;
  delete h_mc_sig;
  delete h_mc_truth;

  return;
}

/*
███████ ██ ██      ██          ██████  ██████  ███████
██      ██ ██      ██          ██   ██ ██   ██ ██
█████   ██ ██      ██          ██████  ██   ██ █████
██      ██ ██      ██          ██      ██   ██ ██
██      ██ ███████ ███████     ██      ██████  ██
*/



void fill_pdf(TTree* tree){
  if(!tree) cout<<"could not read 'mc signal' tree\n";
  else      cout << "Filling Model SYS Histograms (PDF) ...\n";


  // setup hist names
  vector<TString> name_sig;
  vector<TString> name_gen;
  vector<TString> name_truth;
  for(unsigned int i=0; i<100; i++){
    TString prefix = "PDF_";
    prefix += i;
    TString ns = prefix + "_sig";
    TString ng = prefix + "_gen";
    TString nt = prefix + "_truth";
    name_sig.push_back(ns);
    name_gen.push_back(ng);
    name_truth.push_back(nt);
  }

  // setup hists
  vector<TH1*> h_mc_sig;
  vector<TH1*> h_mc_gen;
  vector<TH1*> h_mc_truth;
  TH1* h_sig = binning_rec->CreateHistogram("PDF_sig");
  TH1* h_gen = binning_gen->CreateHistogram("PDF_gen");
  // TH1* h_truth = measurement_gen->CreateHistogram("PDF_truth",kTRUE,0,0,axissteer);
  TH1* h_truth = measurement_gen20->CreateHistogram("PDF_truth",kTRUE,0,0,axissteer);
  for(unsigned int i=0; i<100; i++){
    h_mc_sig.push_back((TH1*)h_sig->Clone());
    h_mc_gen.push_back((TH1*)h_gen->Clone());
    h_mc_truth.push_back((TH1*)h_truth->Clone());
  }



  vector<double> *pdf_weights = new vector<double>(100);

  outputFile->cd();
  tree->ResetBranchAddresses();
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Mass_Gen33",&massGen);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("Pt_Gen33",&ptGen);
  // tree->SetBranchAddress("matched",&merged);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("passed_measurement_gen",&passed_measurement_gen);
  tree->SetBranchAddress("is_TTbar",&is_TTbar);
  tree->SetBranchAddress("rec_weight",&rec_weight);
  tree->SetBranchAddress("gen_weight",&gen_weight);
  tree->SetBranchAddress("pdf_weights",&pdf_weights);
  tree->SetBranchAddress("gen_weight_ttfactor",&gen_ttfactor);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_ptmigration_gen",&passed_ptmigration_gen);
  tree->SetBranchAddress("passed_subptmigration_rec",&passed_subptmigration_rec);
  tree->SetBranchAddress("passed_subptmigration_gen",&passed_subptmigration_gen);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_massmigration_gen",&passed_massmigration_gen);
  tree->SetBranchAddress("passed_leptonptmigration_rec",&passed_leptonptmigration_rec);
  tree->SetBranchAddress("passed_leptonptmigration_gen",&passed_leptonptmigration_gen);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);
  tree->SetBranchStatus("*",1);


  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;
    if(!is_TTbar){
      cout << "ERROR: non-ttbar event in signal sample!" << endl;
      break;
    }

    if(ttweight) gen_weight *= gen_ttfactor;

    // get weight for gen and rec hists
    w_sig_rec = rec_weight * gen_weight;
    w_gen = gen_weight;


    // get global bins
    Int_t genBin;
    if     (passed_measurement_gen)   genBin = measurement_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_ptmigration_gen)   genBin = ptmigration_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_subptmigration_gen)genBin = subptmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_massmigration_gen) genBin = massmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_leptonptmigration_gen)genBin = leptonptmigration_gen->GetGlobalBinNumber(massGen);
    else                              genBin = 0;

    Int_t recBin;
    if     (passed_measurement_rec)    recBin = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    recBin = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_subptmigration_rec) recBin = subptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_massmigration_rec)  recBin = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  recBin = btagmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_leptonptmigration_rec) recBin = leptonptmigration_rec->GetGlobalBinNumber(massRec);
    else                               recBin = 0;

    bool gen_info = false;
    bool rec_info = false;
    if(passed_ptmigration_gen || passed_measurement_gen || passed_massmigration_gen || passed_subptmigration_gen || passed_leptonptmigration_gen) gen_info = true;
    if(passed_ptmigration_rec || passed_measurement_rec || passed_massmigration_rec || passed_subptmigration_rec || passed_leptonptmigration_rec || passed_btagmigration_rec) rec_info = true;


    // Fill 1D Hists (for truth distribution only use gen selection)
    for(unsigned int i=0; i<100; i++){
      if(passed_measurement_gen) h_mc_truth[i]->Fill(massGen, w_gen*(*pdf_weights)[i]);
      if(rec_info) h_mc_sig[i]->Fill(recBin, w_sig_rec*(*pdf_weights)[i]);
      if(gen_info) h_mc_gen[i]->Fill(genBin, w_gen*(*pdf_weights)[i]);
    }

  }
  for(unsigned int i=0; i<100; i++){
    h_mc_sig[i]->Write(name_sig[i]);
    h_mc_truth[i]->Write(name_truth[i]);
    h_mc_gen[i]->Write(name_gen[i]);
  }

  return;
}

/*
███████ ██ ██      ██          ██████      ██████  ███████ ██     ██ ███████ ██  ██████  ██   ██ ████████
██      ██ ██      ██          ██   ██     ██   ██ ██      ██     ██ ██      ██ ██       ██   ██    ██
█████   ██ ██      ██          ██████      ██████  █████   ██  █  ██ █████   ██ ██   ███ ███████    ██
██      ██ ██      ██          ██   ██     ██   ██ ██      ██ ███ ██ ██      ██ ██    ██ ██   ██    ██
██      ██ ███████ ███████     ██████      ██   ██ ███████  ███ ███  ███████ ██  ██████  ██   ██    ██
*/

void fill_breweight(TTree* tree, TString prefix){
  if(!tree) cout<<"could not read 'mc signal' tree\n";
  else      cout << "Filling Model SYS Histograms ("+prefix+") ...\n";

  // setup hists
  TH1* h_mc_sig = binning_rec->CreateHistogram(prefix + "_sig");
  TH1* h_mc_gen = binning_gen->CreateHistogram(prefix + "_gen");
  // TH1* h_mc_truth = measurement_gen->CreateHistogram(prefix + "_truth",kTRUE,0,0,axissteer);
  TH1* h_mc_truth = measurement_gen20->CreateHistogram(prefix + "_truth",kTRUE,0,0,axissteer);

  outputFile->cd();
  tree->ResetBranchAddresses();
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Mass_Gen33",&massGen);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("Pt_Gen33",&ptGen);
  // tree->SetBranchAddress("matched",&merged);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("passed_measurement_gen",&passed_measurement_gen);
  tree->SetBranchAddress("is_TTbar",&is_TTbar);
  tree->SetBranchAddress("rec_weight",&rec_weight);
  tree->SetBranchAddress("gen_weight",&gen_weight);
  tree->SetBranchAddress("gen_weight_ttfactor",&gen_ttfactor);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_ptmigration_gen",&passed_ptmigration_gen);
  tree->SetBranchAddress("passed_subptmigration_rec",&passed_subptmigration_rec);
  tree->SetBranchAddress("passed_subptmigration_gen",&passed_subptmigration_gen);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_massmigration_gen",&passed_massmigration_gen);
  tree->SetBranchAddress("passed_leptonptmigration_rec",&passed_leptonptmigration_rec);
  tree->SetBranchAddress("passed_leptonptmigration_gen",&passed_leptonptmigration_gen);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);
  tree->SetBranchAddress("bquark_pt",&pt_b);
  tree->SetBranchStatus("*",1);


  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;
    if(!is_TTbar){
      cout << "ERROR: non-ttbar event in signal sample!" << endl;
      break;
    }

    if(ttweight) gen_weight *= gen_ttfactor;
    double maxdeviation = 0.1;
    double crossing = 165;
    double slope = 0.01;
    int sign = 1.0;
    if(prefix == "breweight_down") sign = -1.0;

    double factor1 = 1.0 + sign * maxdeviation * tanh( ((*pt_b)[0]-crossing)*slope );
    double factor2 = 1.0 + sign * maxdeviation * tanh( ((*pt_b)[1]-crossing)*slope );

    gen_weight *= (factor1*factor2);

    // get weight for gen and rec hists
    w_sig_rec = rec_weight * gen_weight;
    w_gen = gen_weight;


    // get global bins
    Int_t genBin;
    if     (passed_measurement_gen)   genBin = measurement_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_ptmigration_gen)   genBin = ptmigration_gen->GetGlobalBinNumber(massGen,ptGen);
    else if(passed_subptmigration_gen)genBin = subptmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_massmigration_gen) genBin = massmigration_gen->GetGlobalBinNumber(massGen);
    else if(passed_leptonptmigration_gen)genBin = leptonptmigration_gen->GetGlobalBinNumber(massGen);
    else                              genBin = 0;

    Int_t recBin;
    if     (passed_measurement_rec)    recBin = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    recBin = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_subptmigration_rec) recBin = subptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_massmigration_rec)  recBin = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_leptonptmigration_rec) recBin = leptonptmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  recBin = btagmigration_rec->GetGlobalBinNumber(massRec);
    else                               recBin = 0;

    bool gen_info = false;
    bool rec_info = false;
    if(passed_ptmigration_gen || passed_measurement_gen || passed_massmigration_gen || passed_subptmigration_gen || passed_leptonptmigration_gen) gen_info = true;
    if(passed_ptmigration_rec || passed_measurement_rec || passed_massmigration_rec || passed_subptmigration_rec || passed_leptonptmigration_rec || passed_btagmigration_rec) rec_info = true;


    // Fill 1D Hists (for truth distribution only use gen selection)
    if(passed_measurement_gen) h_mc_truth->Fill(massGen, w_gen);
    if(rec_info) h_mc_sig->Fill(recBin, w_sig_rec);
    if(gen_info) h_mc_gen->Fill(genBin, w_gen);


  }

  h_mc_sig->Write();
  h_mc_truth->Write();
  h_mc_gen->Write();

  delete h_mc_gen;
  delete h_mc_sig;
  delete h_mc_truth;

  return;

}

void fill_additionaltemplates(TString channel, TString year){
  cout << "Filling Additional Mass Templates ...\n";

  // now fill hist for every bin
  vector<double> masses = {169.5, 171.5, 173.5, 175.5};
  vector<TGraphErrors*> binhists;

  int nbins = h_mtop_truth[0]->GetXaxis()->GetNbins();
  for(int bin=1; bin<=nbins; bin++){
    vector<double> contents;
    vector<double> x_errors;
    vector<double> y_errors;
    for(unsigned int i=0; i<h_mtop_truth.size(); i++){
      contents.push_back(h_mtop_truth[i]->GetBinContent(bin));
      x_errors.push_back(1.0);
      y_errors.push_back(h_mtop_truth[i]->GetBinError(bin));
    }
    TGraphErrors *h = new TGraphErrors(h_mtop_truth.size(), &masses[0], &contents[0], &x_errors[0], &y_errors[0]);
    binhists.push_back(h);
  }

  vector<TF1*> fits;
  for(unsigned int i=0; i<binhists.size(); i++){
    TF1*f1 = new TF1("f1","pol1",160,180);
    // some calculations estimating fit parameters for good starting values
    double v1 = binhists[i]->Eval(170);
    double v2 = binhists[i]->Eval(175);
    double par2_est = (v2-v1)/5;
    double par1_est = v1 - par2_est*170;
    f1->SetParameter(0,par1_est);
    f1->SetParameter(1,par2_est);
    ////
    binhists[i]->Fit("f1","R");
    TF1* fit = binhists[i]->GetFunction("f1");
    fits.push_back(fit);
  }

  PlotBinHists(binhists, fits, channel, year);

  TH1* mtop1700 = CreateNewSample(fits, 170.0, h_mtop_truth[0], "mc_mtop1700_truth");
  TH1* mtop1705 = CreateNewSample(fits, 170.5, h_mtop_truth[0], "mc_mtop1705_truth");
  TH1* mtop1710 = CreateNewSample(fits, 171.0, h_mtop_truth[0], "mc_mtop1710_truth");
  TH1* mtop1720 = CreateNewSample(fits, 172.0, h_mtop_truth[0], "mc_mtop1720_truth");
  TH1* mtop1730 = CreateNewSample(fits, 173.0, h_mtop_truth[0], "mc_mtop1730_truth");
  TH1* mtop1740 = CreateNewSample(fits, 174.0, h_mtop_truth[0], "mc_mtop1740_truth");
  TH1* mtop1745 = CreateNewSample(fits, 174.5, h_mtop_truth[0], "mc_mtop1745_truth");
  TH1* mtop1750 = CreateNewSample(fits, 175.0, h_mtop_truth[0], "mc_mtop1750_truth");

  outputFile->cd();
  mtop1700->Write("mc_mtop1700_truth");
  mtop1705->Write("mc_mtop1705_truth");
  mtop1710->Write("mc_mtop1710_truth");
  mtop1720->Write("mc_mtop1720_truth");
  mtop1730->Write("mc_mtop1730_truth");
  mtop1740->Write("mc_mtop1740_truth");
  mtop1745->Write("mc_mtop1745_truth");
  mtop1750->Write("mc_mtop1750_truth");
}


TH1* CreateNewSample(vector<TF1*> fits, double mass, TH1* dummy, TString name){
  TH1* sample = (TH1*) dummy->Clone(name);
  sample->Reset();
  sample->SetTitle(name);
  for(unsigned int i=0; i<fits.size(); i++){
    int bin = i+1;
    double content = fits[i]->Eval(mass);
    sample->SetBinContent(bin, content);
  }
  return sample;
}

void PlotBinHists(vector<TGraphErrors*> binhists, vector<TF1*> fits, TString channel, TString year){
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  gStyle->SetLegendBorderSize(0);

  for(unsigned int i=0; i<binhists.size(); i++){
    int binnr = i+1;
    TCanvas *c= new TCanvas(" ","",600,600);
    gPad->SetLeftMargin(0.15);
    binhists[i]->SetTitle(" ");
    binhists[i]->GetXaxis()->SetTitle("m_{t} [GeV]");
    binhists[i]->GetYaxis()->SetTitle("bin content");
    binhists[i]->GetYaxis()->SetTitleOffset(1.5);
    binhists[i]->GetYaxis()->SetNdivisions(505);
    binhists[i]->SetLineColor(kBlack);
    binhists[i]->SetMarkerColor(kBlack);
    binhists[i]->SetMarkerStyle(20);
    binhists[i]->Draw("AP");
    fits[i]->SetLineColor(kRed);
    fits[i]->Draw("SAME");
    gPad->RedrawAxis();
    TString savename = "/afs/desy.de/user/s/schwarzd/Plots/ExtrapolateMTop_Run2/"+year+"_"+channel+"_bin_";
    savename += binnr;
    savename += ".pdf";
    c->SaveAs(savename);
    delete c;
  }
}
