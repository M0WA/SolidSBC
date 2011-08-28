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
    public partial class ClientAnalyzerForm : Form
    {
        private BindingSource bindingSource;

        public ClientAnalyzerForm(ref BindingSource bindingSource)
        {
            InitializeComponent();
            this.bindingSource = bindingSource;
        }

        private void ClientAnalyzerForm_Load(object sender, EventArgs e)
        {
        }

        private void dataGridViewClientList_SelectionChanged(object sender, EventArgs e)
        {
        }
    }
}
