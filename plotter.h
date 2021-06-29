#include <TMath.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TSpline.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TMarker.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>
#include <TVectorD.h>
#include <TF1.h>
#include <vector>
#include <TLatex.h>
#include <fstream>

class plotter{

 public:
  plotter(TString, TString);
  void draw_matrix(TH2* hist, TString file_name, bool zlog, bool is_migration);
  void draw_output(TH1* output, TH1D* truth, bool norm, TString file_name);
  void draw_output_fine(TH1* output, bool norm, TString file_name);
  void draw_output_data(TH1* output_, TH1* stat_, std::vector<TH1D*> truth_, std::vector<TString> legnames, bool norm, TString file_name);
  void draw_output_smear(std::vector<TH1*> output, TH1D* truth, TString file_name);
  void draw_output_stat(TH1* output_, TH1* stat_, TH1D* truth_, bool norm, TString file_name);
  void draw_output_mass(TH1* output, TH1* stat_, std::vector<TH1D*> mtop_templates, std::vector<bool> show, bool norm, TString file_name);
  void draw_output_pseudo(TH1* output, TH1D* pseudotruth, TH1D* mctruth, bool norm, TString file_name);
  void draw_lcurve(TGraph *lcurve, double x1, double y1,  double x2, double y2, TString file_name);
  void draw_rhotau(TSpline *rhotau, double tau, TString file_name);
  void draw_projection(TH1D* proj, TH1D* compare, TString file_name );
  void draw_1D_hist(TH1D* hist, TString file_name);
  void draw_binhist(TGraphErrors* hist, TString file_name, double xmin, double xmax, int power);
  void draw_delta(TH1* hist, TString file_name);
  void draw_delta_rel(TH1* hist, TH1* result, TString file_name);
  void draw_rec(TH1D* data, TH1D* sig, TH1D* bgr, TString file_name);
  void draw_purity(TH1D* numerator, TH1D* denominator, TString file_name);
  void draw_stability(TH1D* numerator, TH1D* denominator, TString file_name);
  TH1D* get_difference(TH1D* hist1, TH1D* hist2);
  void draw_chi2(TF1 * fit, std::vector<double> masses, std::vector<double> chi2, double mass, double uncert, TString file_name);
  void draw_delta_comparison( TH1* total_, TH1* stat_, std::vector<TH1*> MODEL_DELTA, std::vector<TString> UncertNames,TString file_name);
  void draw_delta_comparison_model( TH1* total_, TH1* stat_, std::vector<TH1*> MODEL_DELTA, std::vector<TString> UncertNames, TString file_name);
  void draw_bias(TH1* output_, TH1D* truth_, TH1* bias_, TString file_name);
  void draw_smearFit(TH1D* variation, TF1* fit_, TString file_name);
  void draw_theo(TH1D* truth, std::vector<TH1D*> variations, bool norm, TString file_name);

 private:
  TH1* add_error_bar(TH1* hist, std::vector<double> errors);
  TString directory;
  TString year;
  void CMSLabel(bool, double x=0.25, double y=0.83);
  void LumiInfo();
  TH1F* GetRatio(TH1F* h1, TH1F* h2);
  TH1F* GetMCRatioUncert(TH1F* mc);

};
