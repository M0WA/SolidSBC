using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SolidSBCResultViewer
{
    public partial class SolidSBCResultViewer : Form
    {
        private DatabaseLoginForm formLogin = null;
        private ClientAnalyzerForm formClientAnalyzer = null;
        private ResultAnalyzerForm formResultAnalyzer = null;

        public SolidSBCResultViewer()
        {
            InitializeComponent();

            SetStatusText("connecting to database...");

            formLogin = new DatabaseLoginForm(ref bindingSource);
            formLogin.LoggedInEvent += new DatabaseLoginForm.LoggedInHandler(LoggedInHandler);
            ShowForm(formLogin);
        }

        ~SolidSBCResultViewer()
        {
            formLogin.LoggedInEvent -= new DatabaseLoginForm.LoggedInHandler(LoggedInHandler);
        }

        private void clientsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (formClientAnalyzer == null || formClientAnalyzer.IsDisposed)
                formClientAnalyzer = new ClientAnalyzerForm(ref bindingSource);
            ShowForm(formClientAnalyzer);
        }

        private void resultsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (formResultAnalyzer == null || formResultAnalyzer.IsDisposed)
                formResultAnalyzer = new ResultAnalyzerForm(ref bindingSource);
            ShowForm(formResultAnalyzer);
        }

        private void connectionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (formLogin == null || formLogin.IsDisposed)
                formLogin = new DatabaseLoginForm(ref bindingSource);
            ShowForm(formLogin);
        }

        private void LoggedInHandler(object sender, DatabaseLoginForm.LoggedInEventArgs e)
        {
            if (!e.IsLoggedIn)
                SetStatusText("not connected to database.");
            else
                SetStatusText("database connected.");
        }

        private void ShowForm(Form form)
        {
            form.MdiParent = this;
            form.Show();
        }

        private void SetStatusText(string sStatusText)
        {
            statusStrip.Items.Clear();
            statusStrip.Items.Add(new ToolStripLabel(sStatusText));
        }
    }
}
