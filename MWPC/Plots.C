void Plots()
{
    gROOT->ForceStyle();
    TFile *fin = new TFile("output.root");

    //--------------------------------------------------
    TCanvas *cA = new TCanvas("cA", "", 3000, 1000);
    cA->Divide(3,1);
    
    TH1F *h110 = (TH1F*)fin->Get("h110");
    TH1F *h111 = (TH1F*)fin->Get("h111");
  
    int lastSpill = 5;
    for(int ib=1; ib<=h110->GetNbinsX(); ib++){
        if(h110->GetBinContent(ib) > 0.5 && h110->GetBinCenter(ib) > lastSpill){
            lastSpill = h110->GetBinCenter(ib);
        }
    }
    lastSpill += 5;
    h110->GetXaxis()->SetRange(1,lastSpill);
    h110->GetXaxis()->SetTitle("spill number");
    h110->GetYaxis()->SetTitle("all events");
    
    cA->cd(1);
    h110->Draw();

    TGraphAsymmErrors *grA = new TGraphAsymmErrors(h111,h110);
    grA->SetMinimum(0);
    grA->SetMarkerStyle(20);
    grA->SetMarkerSize(2);
    grA->SetLineWidth(2);
    grA->SetLineColor(1);
    grA->SetMarkerColor(1);
    grA->GetXaxis()->SetTitle("spill number");
    grA->GetYaxis()->SetTitle("fraction of events with non-empty MWPC");
    grA->GetXaxis()->SetLimits(-0.5, lastSpill+0.5);

    cA->cd(2);
    grA->Draw("AP");
        
    TH1F *h02 = (TH1F*)fin->Get("h02");
    h02->GetXaxis()->SetTitle("MWPC module");
    h02->GetYaxis()->SetTitle("hits");
    
    cA->cd(3);
    h02->Draw();
 
    
    
    //--------------------------------------------------
    TCanvas *cB = new TCanvas("cB", "", 3000, 2000);
    cB->Divide(3,2);
    
    TH2F *h1XA = (TH2F*)fin->Get("TDC_vs_X_1");
    h1XA->GetXaxis()->SetTitle("WC1X channel");
    h1XA->GetYaxis()->SetTitle("WC1X TDC");
    h1XA->GetZaxis()->SetTitle("WC1X hits");
    cB->cd(1);
    h1XA->Draw("colz");
    
    TH1F *h1XB = (TH1F*)fin->Get("h102_1");
    h1XB->GetXaxis()->SetTitle("WC1X channel");
    h1XB->GetYaxis()->SetTitle("events");
    cB->cd(2);
    h1XB->Draw("hist");
    
    TH1F *h1XC = (TH1F*)fin->Get("h103_1");
    h1XC->GetXaxis()->SetTitle("WC1X earliest TDC");
    h1XC->GetYaxis()->SetTitle("events");
    cB->cd(3);
    h1XC->Draw("hist");
    
    TH2F *h1YA = (TH2F*)fin->Get("TDC_vs_Y_1");
    h1YA->GetXaxis()->SetTitle("WC1Y channel");
    h1YA->GetYaxis()->SetTitle("WC1Y TDC");
    h1YA->GetZaxis()->SetTitle("WC1Y hits");
    cB->cd(4);
    h1YA->Draw("colz");
    
    TH1F *h1YB = (TH1F*)fin->Get("h104_1");
    h1YB->GetXaxis()->SetTitle("WC1Y channel");
    h1YB->GetYaxis()->SetTitle("events");
    cB->cd(5);
    h1YB->Draw("hist");
    
    TH1F *h1YC = (TH1F*)fin->Get("h105_1");
    h1YC->GetXaxis()->SetTitle("WC1Y earliest TDC");
    h1YC->GetYaxis()->SetTitle("events");
    cB->cd(6);
    h1YC->Draw("hist");
    
    //--------------------------------------------------
    TCanvas *cC = new TCanvas("cC", "", 3000, 2000);
    cC->Divide(3,2);
    
    TH2F *h2XA = (TH2F*)fin->Get("TDC_vs_X_2");
    h2XA->GetXaxis()->SetTitle("WC2X channel");
    h2XA->GetYaxis()->SetTitle("WC2X TDC");
    h2XA->GetZaxis()->SetTitle("WC2X hits");
    cC->cd(1);
    h2XA->Draw("colz");
    
    TH1F *h2XB = (TH1F*)fin->Get("h102_2");
    h2XB->GetXaxis()->SetTitle("WC2X channel");
    h2XB->GetYaxis()->SetTitle("events");
    cC->cd(2);
    h2XB->Draw("hist");
    
    TH1F *h2XC = (TH1F*)fin->Get("h103_2");
    h2XC->GetXaxis()->SetTitle("WC2X earliest TDC");
    h2XC->GetYaxis()->SetTitle("events");
    cC->cd(3);
    h2XC->Draw("hist");
    
    TH2F *h2YA = (TH2F*)fin->Get("TDC_vs_Y_2");
    h2YA->GetXaxis()->SetTitle("WC2Y channel");
    h2YA->GetYaxis()->SetTitle("WC2Y TDC");
    h2YA->GetZaxis()->SetTitle("WC2Y hits");
    cB->cd(4);
    h2YA->Draw("colz");
    
    TH1F *h2YB = (TH1F*)fin->Get("h104_2");
    h2YB->GetXaxis()->SetTitle("WC2Y channel");
    h2YB->GetYaxis()->SetTitle("events");
    cC->cd(5);
    h2YB->Draw("hist");
    
    TH1F *h2YC = (TH1F*)fin->Get("h105_2");
    h2YC->GetXaxis()->SetTitle("WC2Y earliest TDC");
    h2YC->GetYaxis()->SetTitle("events");
    cC->cd(6);
    h2YC->Draw("hist");
    
    //--------------------------------------------------
    TCanvas *cD = new TCanvas("cD", "", 3000, 2000);
    cD->Divide(3,2);
    
    TH2F *h3XA = (TH2F*)fin->Get("TDC_vs_X_3");
    h3XA->GetXaxis()->SetTitle("WC3X channel");
    h3XA->GetYaxis()->SetTitle("WC3X TDC");
    h3XA->GetZaxis()->SetTitle("WC3X hits");
    cD->cd(1);
    h3XA->Draw("colz");
    
    TH1F *h3XB = (TH1F*)fin->Get("h102_3");
    h3XB->GetXaxis()->SetTitle("WC3X channel");
    h3XB->GetYaxis()->SetTitle("events");
    cD->cd(2);
    h3XB->Draw("hist");
    
    TH1F *h3XC = (TH1F*)fin->Get("h103_3");
    h3XC->GetXaxis()->SetTitle("WC3X earliest TDC");
    h3XC->GetYaxis()->SetTitle("events");
    cD->cd(3);
    h3XC->Draw("hist");
    
    TH2F *h3YA = (TH2F*)fin->Get("TDC_vs_Y_3");
    h3YA->GetXaxis()->SetTitle("WC3Y channel");
    h3YA->GetYaxis()->SetTitle("WC3Y TDC");
    h3YA->GetZaxis()->SetTitle("WC3Y hits");
    cD->cd(4);
    h3YA->Draw("colz");
    
    TH1F *h3YB = (TH1F*)fin->Get("h104_3");
    h3YB->GetXaxis()->SetTitle("WC3Y channel");
    h3YB->GetYaxis()->SetTitle("events");
    cD->cd(5);
    h3YB->Draw("hist");
    
    TH1F *h3YC = (TH1F*)fin->Get("h105_3");
    h3YC->GetXaxis()->SetTitle("WC3Y earliest TDC");
    h3YC->GetYaxis()->SetTitle("events");
    cD->cd(6);
    h3YC->Draw("hist");
    
    //--------------------------------------------------
    TCanvas *cE = new TCanvas("cE", "", 3000, 2000);
    cE->Divide(3,2);
    
    TH2F *h4XA = (TH2F*)fin->Get("TDC_vs_X_4");
    h4XA->GetXaxis()->SetTitle("WC4X channel");
    h4XA->GetYaxis()->SetTitle("WC4X TDC");
    h4XA->GetZaxis()->SetTitle("WC4X hits");
    cE->cd(1);
    h4XA->Draw("colz");
    
    TH1F *h4XB = (TH1F*)fin->Get("h102_4");
    h4XB->GetXaxis()->SetTitle("WC4X channel");
    h4XB->GetYaxis()->SetTitle("events");
    cE->cd(2);
    h4XB->Draw("hist");
    
    TH1F *h4XC = (TH1F*)fin->Get("h103_4");
    h4XC->GetXaxis()->SetTitle("WC4X earliest TDC");
    h4XC->GetYaxis()->SetTitle("events");
    cE->cd(3);
    h4XC->Draw("hist");
    
    TH2F *h4YA = (TH2F*)fin->Get("TDC_vs_Y_4");
    h4YA->GetXaxis()->SetTitle("WC4Y channel");
    h4YA->GetYaxis()->SetTitle("WC4Y TDC");
    h4YA->GetZaxis()->SetTitle("WC4Y hits");
    cE->cd(4);
    h4YA->Draw("colz");
    
    TH1F *h4YB = (TH1F*)fin->Get("h104_4");
    h4YB->GetXaxis()->SetTitle("WC4Y channel");
    h4YB->GetYaxis()->SetTitle("events");
    cE->cd(5);
    h4YB->Draw("hist");
    
    TH1F *h4YC = (TH1F*)fin->Get("h105_4");
    h4YC->GetXaxis()->SetTitle("WC4Y earliest TDC");
    h4YC->GetYaxis()->SetTitle("events");
    cE->cd(6);
    h4YC->Draw("hist");
    
}
