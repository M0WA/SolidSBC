using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SolidSBCResultViewer
{
    public partial class DatabaseLoginForm : Form
    {
        public class LoggedInEventArgs : EventArgs
        {
            public LoggedInEventArgs(Boolean isLoggedIn)
            {
                this.isLoggedIn = isLoggedIn;
            }

            private Boolean isLoggedIn = false;
            public Boolean IsLoggedIn
            {
                get { return isLoggedIn; }
            }
        }

        public delegate void LoggedInHandler(object sender, LoggedInEventArgs e);
        public event LoggedInHandler LoggedInEvent;

        private BindingSource bindingSource;

        public DatabaseLoginForm(ref BindingSource bindingSource)
        {
            InitializeComponent();
            this.bindingSource = bindingSource;
        }

        private void buttonLogin_Click(object sender, EventArgs e)
        {
            Boolean bIsLoggedIn = true;
            if (bIsLoggedIn)
            {
                buttonLogin.Text = "logout";

                Hide();

                if (LoggedInEvent != null)
                    LoggedInEvent(this, new LoggedInEventArgs(true));
            }
            else
            {
                buttonLogin.Text = "login";

                if (LoggedInEvent != null)
                    LoggedInEvent(this, new LoggedInEventArgs(false));
            }
        }
    }
}
