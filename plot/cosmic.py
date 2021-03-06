from rootalias import *
import numpy as np

f_data = TFile('photon_timing.fd_cosmic_data.root')
f_mc = TFile('photon_timing.fd_cosmic_mc.root')
figure_dir = '/Users/juntinghuang/google_drive/slides/beamer/20170621_cosmic_timing/figures'

def plot_slice_duration(width, max_x):
    h_data = f_data.Get('photontimingcosmicana/fSliceDuration{}'.format(width))
    h_mc = f_mc.Get('photontimingcosmicana/fSliceDuration{}'.format(width))
    h_data.Scale(1.0 / h_data.Integral())
    h_mc.Scale(1.0 / h_mc.Integral())

    gStyle.SetOptStat('nmr')
    c1 = TCanvas('c1', 'c1', 800, 600)
    set_margin()

    x_title = 'Timing Width of Center {}% Hits (ns)'.format(width)
    if width == '':
        width = '100'
        x_title = 'Timing Width from All Hits (ns)'

    set_h1_style(h_data)
    h_data.SetName('FD Cosmic Data')
    h_data.GetXaxis().SetTitle(x_title)
    h_data.GetYaxis().SetTitle('Track Count (area norm.)')
    h_data.GetYaxis().SetTitleOffset(1.6)
    h_data.SetLineColor(kRed)
    h_data.GetXaxis().SetRangeUser(0, max_x)
    h_data.GetYaxis().SetRangeUser(0, max(h_data.GetMaximum(), h_mc.GetMaximum()) * 1.1)
    h_data.Draw('hist')

    set_h1_style(h_mc)
    h_mc.SetName('FD Cosmic MC')
    h_mc.Draw('sames, hist')

    c1.Update()
    p_data = h_data.GetListOfFunctions().FindObject("stats")
    p_data.SetTextColor(h_data.GetLineColor())
    p_data.SetLineColor(h_data.GetLineColor())
    p_data.SetX1NDC(0.7)
    p_data.SetY1NDC(0.75)
    p_data.SetX2NDC(0.95)
    p_data.SetY2NDC(0.95)
    p_data.Draw()

    p_mc = h_mc.GetListOfFunctions().FindObject("stats")
    p_mc.SetTextColor(h_mc.GetLineColor())
    p_mc.SetLineColor(h_mc.GetLineColor())
    p_mc.SetX1NDC(0.7)
    p_mc.SetY1NDC(0.51)
    p_mc.SetX2NDC(0.95)
    p_mc.SetY2NDC(0.71)
    p_mc.Draw()

    c1.Update()
    c1.SaveAs('{}/plot_slice_duration.width_{}.pdf'.format(figure_dir, width))
    raw_input('Press any key to continue.')

def plot_cell_count():
    h_data_ncell = f_data.Get('photontimingcosmicana/fCellCountPerSlice')
    h_mc_ncell = f_mc.Get('photontimingcosmicana/fCellCountPerSlice')
    h_data_ncell.Scale(1.0 / h_data_ncell.Integral())
    h_mc_ncell.Scale(1.0 / h_mc_ncell.Integral())

    gStyle.SetOptStat(0)
    c3 = TCanvas('c3', 'c3', 800, 600)
    gPad.SetBottomMargin(0.15)
    gPad.SetLeftMargin(0.15)

    pad1 = TPad("pad1", "pad1", 0, 0.4, 1, 1)
    pad1.SetTopMargin(0.15)
    pad1.SetBottomMargin(0)
    pad1.SetLeftMargin(0.15)
    pad1.Draw()
    pad1.cd()
    set_h1_style(h_data_ncell)
    h_data_ncell.SetLineColor(kRed)
    h_data_ncell.SetMarkerColor(kRed)
    h_data_ncell.GetYaxis().SetTitle('Track Count (area norm.)')
    h_data_ncell.GetYaxis().SetRangeUser(-0.02, max(h_data_ncell.GetMaximum(), h_mc_ncell.GetMaximum()) * 1.2)
    h_data_ncell.GetXaxis().SetRangeUser(0.0, 30)
    h_data_ncell.GetYaxis().SetTitleOffset(1.5)
    h_data_ncell.Draw("hist")

    set_h1_style(h_mc_ncell)
    h_mc_ncell.Draw("sames, hist")

    lg1 = TLegend(0.7, 0.5, 0.9, 0.8)
    set_legend_style(lg1)
    lg1.AddEntry(h_data_ncell, 'data', 'l')
    lg1.AddEntry(h_mc_ncell, 'mc', 'l')
    lg1.Draw()

    c3.cd()
    pad2 = TPad('pad2', 'pad2', 0, 0, 1, 0.4)
    pad2.SetTopMargin(0)
    pad2.SetLeftMargin(0.15)
    pad2.SetBottomMargin(0.4)
    pad2.Draw()
    pad2.cd()
    gPad.SetGrid()

    h_ratio = h_data_ncell.Clone()
    h_ratio.Sumw2()
    h_ratio.Divide(h_mc_ncell)
    h_ratio.GetYaxis().SetRangeUser(0., 2.2)
    h_ratio.SetLineColor(kBlack)
    h_ratio.SetTitle('')
    h_ratio.GetYaxis().SetNdivisions(205, 1)
    h_ratio.GetXaxis().SetTitle('Selected Cell Hit Count per Track')
    h_ratio.GetYaxis().SetTitle('data / MC')
    h_ratio.GetXaxis().SetTitleOffset(3)
    h_ratio.Draw('ep')

    c3.Update()
    c3.SaveAs('figures/plot_cell_count.cosmic.pdf')
    raw_input('Press any key to continue.')

def plot_cell_hits(view, tfile, suffix):
    cells = []
    planes = []
    selected_cells = []
    selected_planes = []

    for cell_hit in tfile.Get('photontimingcosmicana/fCellHitTree'):
        if (cell_hit.cellHitView == view):
            cells.append(float(cell_hit.cellHitCell))
            planes.append(float(cell_hit.cellHitPlane))
            if cell_hit.cellHitSelected == 1:
                selected_cells.append(float(cell_hit.cellHitCell))
                selected_planes.append(float(cell_hit.cellHitPlane))
        if len(selected_cells) == 200:
            break

    gr_cell_plane = TGraph(len(cells), np.asarray(planes), np.asarray(cells))
    gr_selected_cell_plane = TGraph(len(selected_cells), np.array(selected_planes), np.array(selected_cells))

    gStyle.SetOptStat(0)
    c1 = TCanvas('c1', 'c1', 800, 800)
    set_margin()

    set_graph_style(gr_cell_plane)
    gr_cell_plane.SetMarkerStyle(20)
    gr_cell_plane.SetMarkerSize(0.3)
    gr_cell_plane.Draw('AP')
    gr_cell_plane.GetXaxis().SetTitle('Plane Number')
    gr_cell_plane.GetYaxis().SetTitle('Cell Number')
    gr_cell_plane.GetYaxis().SetRangeUser(0, 480)
    gr_cell_plane.GetXaxis().SetTitleOffset(1.4)
    gr_cell_plane.GetYaxis().SetTitleOffset(2)

    set_graph_style(gr_selected_cell_plane)
    gr_selected_cell_plane.SetMarkerStyle(20)
    gr_selected_cell_plane.SetMarkerSize(0.35)
    gr_selected_cell_plane.SetMarkerColor(kRed)
    gr_selected_cell_plane.Draw('P')

    lg1 = TLegend(0.4, 0.8, 0.88, 0.88)
    set_legend_style(lg1)
    lg1.SetFillStyle(1001)
    lg1.SetBorderSize(1)
    lg1.SetMargin(0.15)
    lg1.AddEntry(gr_cell_plane, 'cell hits', 'p')
    lg1.AddEntry(gr_selected_cell_plane, 'cell hits in range', 'p')
    lg1.SetNColumns(2)
    lg1.Draw()

    c1.Update()
    c1.SaveAs('figures/plot_cell_hits_{}.{}.pdf'.format('x_view' if view == 0 else 'y_view', suffix))
    raw_input('Press any key to continue.')

def plot_tns_distribution():
    h_data = f_data.Get('photontimingcosmicana/fSliceCellHitTns')
    h_mc = f_mc.Get('photontimingcosmicana/fSliceCellHitTns')
    h_data.Scale(1.0 / h_data.Integral())
    h_mc.Scale(1.0 / h_mc.Integral())

    c1 = TCanvas('c1', 'c1', 800, 600)
    set_margin()
    set_h1_style(h_data)
    h_data.SetLineColor(kRed)
    h_data.GetXaxis().SetRangeUser(1, 500)
    h_data.GetYaxis().SetRangeUser(0, 0.01)
    h_data.SetName('FD Cosmic Data')
    h_data.GetXaxis().SetTitle('Time to First Selected Cell Hit (ns)')
    h_data.GetYaxis().SetTitle('Cell Hit Count')
    h_data.GetYaxis().SetTitleOffset(1.6)
    h_data.Draw('hist')

    set_h1_style(h_mc)
    h_mc.SetName('FD Cosmic MC')
    h_mc.Draw('sames, hist')

    c1.Update()
    p_data = h_data.GetListOfFunctions().FindObject("stats")
    p_data.SetTextColor(h_data.GetLineColor())
    p_data.SetLineColor(h_data.GetLineColor())
    p_data.SetX1NDC(0.7)
    p_data.SetY1NDC(0.75)
    p_data.SetX2NDC(0.95)
    p_data.SetY2NDC(0.95)
    p_data.Draw()

    p_mc = h_mc.GetListOfFunctions().FindObject("stats")
    p_mc.SetTextColor(h_mc.GetLineColor())
    p_mc.SetLineColor(h_mc.GetLineColor())
    p_mc.SetX1NDC(0.7)
    p_mc.SetY1NDC(0.51)
    p_mc.SetX2NDC(0.95)
    p_mc.SetY2NDC(0.71)
    p_mc.Draw()

    c1.Update()
    c1.SaveAs('{}/plot_tns_distribution.pdf'.format(figure_dir))
    raw_input('Press any key to continue.')

def plot_tns_distribution_ratio():
    h_data = f_data.Get('photontimingcosmicana/fSliceCellHitTns')
    h_mc = f_mc.Get('photontimingcosmicana/fSliceCellHitTns')
    h_data.Scale(1.0 / h_data.Integral())
    h_mc.Scale(1.0 / h_mc.Integral())

    gStyle.SetOptStat(0)
    c1 = TCanvas('c1', 'c1', 800, 600)
    gPad.SetBottomMargin(0.15)
    gPad.SetLeftMargin(0.15)

    pad1 = TPad("pad1", "pad1", 0, 0.4, 1, 1)
    pad1.SetTopMargin(0.15)
    pad1.SetBottomMargin(0)
    pad1.SetLeftMargin(0.15)
    pad1.Draw()
    pad1.cd()
    set_h1_style(h_data)
    h_data.SetLineColor(kRed)
    h_data.SetMarkerColor(kRed)
    h_data.GetYaxis().SetTitle('Cell Hit Count')
    h_data.GetXaxis().SetRangeUser(0, 300)
    h_data.GetYaxis().SetRangeUser(-0.0005, 0.01)
    h_data.GetYaxis().SetTitleOffset(1.6)
    h_data.Draw("hist")
    set_h1_style(h_mc)
    h_mc.Draw("sames, hist")
    lg1 = TLegend(0.7, 0.5, 0.9, 0.8)
    set_legend_style(lg1)
    lg1.AddEntry(h_data, 'data', 'l')
    lg1.AddEntry(h_mc, 'mc', 'l')
    lg1.Draw()

    c1.cd()
    pad2 = TPad('pad2', 'pad2', 0, 0, 1, 0.4)
    pad2.SetTopMargin(0)
    pad2.SetLeftMargin(0.15)
    pad2.SetBottomMargin(0.4)
    pad2.Draw()
    pad2.cd()
    gPad.SetGrid()
    h_ratio = h_data.Clone()
    h_ratio.Sumw2()
    h_ratio.Divide(h_mc)
    h_ratio.GetYaxis().SetRangeUser(0.7, 1.3)
    h_ratio.SetLineColor(kBlack)
    h_ratio.SetMarkerColor(kBlack)
    h_ratio.SetTitle('')
    h_ratio.GetYaxis().SetNdivisions(205, 1)
    h_ratio.GetXaxis().SetTitle('Time to First Selected Cell Hit (ns)')
    h_ratio.GetYaxis().SetTitle('data / MC')
    h_ratio.GetXaxis().SetTitleOffset(3)
    h_ratio.Draw('ep')

    c1.Update()
    c1.SaveAs('{}/plot_tns_distribution_ratio.pdf'.format(figure_dir))
    raw_input('Press any key to continue.')

# plot_slice_duration('60', 300)
# plot_slice_duration('70', 400)
# plot_slice_duration('80', 500)
# plot_slice_duration('90', 600)
plot_slice_duration('', 700)
# plot_cell_count()
# plot_cell_hits(0, TFile('fardet_r00022336_s61_t02_development_v1_data.fd_cosmic_data.root'), 'data')
# plot_cell_hits(1, TFile('fardet_r00022336_s61_t02_development_v1_data.fd_cosmic_data.root'), 'data')
# plot_cell_hits(0, TFile('fardet_cry_all_r00022747_s17_c000_development_v2_20170413_152427_sim.fd_cosmic_mc.root'), 'mc')
# plot_cell_hits(1, TFile('fardet_cry_all_r00022747_s17_c000_development_v2_20170413_152427_sim.fd_cosmic_mc.root'), 'mc')
# plot_tns_distribution()
# plot_tns_distribution_ratio()
