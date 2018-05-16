using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace JSB_LevelEditor
{
    public partial class ItemExplorer : Form
    {
        public ItemExplorer(ref List<Item> itemList)
        {
            InitializeComponent();
            this.dataGridView1.DataSource = itemList;

            CurrencyManager currencyManager = (CurrencyManager)BindingContext[dataGridView1.DataSource];

            for (int i = 0; i < itemList.Count(); i++)
            {
                if (itemList[i].TextureNumber == 0)
                {
                    currencyManager.SuspendBinding();
                    currencyManager.ResumeBinding();                    
                }
                dataGridView1.Refresh();
            }    
            
        }
    }
}
