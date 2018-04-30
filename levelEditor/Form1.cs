using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections.ObjectModel;
using System.IO;

// TODO: Tasks here
// Refactor
// Erase bug fixed at some point
// Add support for pickups
// Add support for stairs - the game supports a specific angle - needs to be calculated in a method
// Consider compile in editor to speed up workflow
// consider play from editor

namespace JSB_LevelEditor
{
    public partial class Form1 : Form
    {
        const int totalPixels = 921;        
        ObservableCollection<PictureBox> pictureBoxList = new ObservableCollection<PictureBox>();
        List<Item> itemList = new List<Item>();
        List<String> textureList = new List<String>();
        List<String> objectList = new List<string>();     

        ObservableCollection<ToolTip> toolTipList = new ObservableCollection<ToolTip>();
        
        static int textureID = 0;
        static float speedX = 0.0f;
        static float speedY = 0.0f;

        static string selectedObject = "";
        static int selectedObjectIndex = 0;
                
        string defaultTexture = "Default_Blank.png";

        List<ComboBoxItem> objectComboBoxList = new List<ComboBoxItem>();
        List<ComboBoxItem> textureComboBoxList = new List<ComboBoxItem>();

        static string outputTextFileName = "output";
        static string levelID = "0";

        public Form1() {           
            InitializeComponent();
            resetEditor();
        }
        
        void resetEditor()
        {
            for (int i = 1; i < totalPixels; i++)
            {
                pictureBoxList.Add((PictureBox)Controls.Find("pictureBox" + i, true)[0]);
                pictureBoxList[i - 1].Tag = i;
                itemList.Add(new Item());

                toolTipList.Add(new ToolTip());
                toolTipList[i - 1].AutoPopDelay = 5000;
                toolTipList[i - 1].InitialDelay = 1000;
                toolTipList[i - 1].ReshowDelay = 500;
                toolTipList[i - 1].ShowAlways = true;
                toolTipList[i - 1].SetToolTip(pictureBoxList[i - 1], "Empty");
            }
            // add event to each 'pixel'
            foreach (var item in pictureBoxList)
            {
                item.SizeMode = PictureBoxSizeMode.StretchImage;
                item.Image = Image.FromFile(defaultTexture);
                item.MouseClick += new MouseEventHandler(PixelClickEvent);
            }

            textureList.Add("Default_Blank.png");
            textureList.Add("brick.png");
            textureList.Add("ball.png");
            textureList.Add("collect.png");
            textureList.Add("platform.png");

            objectList.Add("Erase");
            objectList.Add("Static Platform");
            objectList.Add("Static Object");
            objectList.Add("Static Stairs");
            objectList.Add("Enemy Static");
            objectList.Add("Enemy Move Horizontal");
            objectList.Add("Enemy Move Vertical");
            objectList.Add("Pick Up");

            for (int i = 0; i < objectList.Count(); i++)
            {
                objectComboBoxList.Add(new ComboBoxItem());
                objectComboBoxList[i].Text = objectList[i];
                this.comboBox1.Items.Add(objectComboBoxList[i]);
            }

            for (int i = 0; i < textureList.Count(); i++)
            {
                textureComboBoxList.Add(new ComboBoxItem());
                textureComboBoxList[i].Text = textureList[i];
                this.comboBox2.Items.Add(textureComboBoxList[i]);
            }

            this.comboBox1.SelectedIndex = 0;
            this.label1.Text = this.comboBox1.SelectedIndex.ToString();

            this.comboBox2.SelectedIndex = 0;
            this.label2.Text = this.comboBox2.SelectedIndex.ToString();
        }

        // event for clicking in a pixel. If it's erase then change texture number to 0 so it is ignored at output (0 is the player sprite in game so is never used)
        private void PixelClickEvent(object sender, MouseEventArgs e) {            
            if (sender is PictureBox pictureBox) {
                pictureBox.Image = Image.FromFile(textureList[textureID]);                
                int i = Int32.Parse(pictureBox.Tag.ToString());
                this.label2.Text = itemList[i - 1].TextureNumber.ToString();
                if (selectedObject == "Erase") {   
                    itemList[i - 1].TextureNumber = 0;
                    itemList[i - 1].ObjectPositionNumber = i - 1;
                    itemList[i - 1].ObjectType = selectedObject;                
                    toolTipList[i - 1].SetToolTip(pictureBoxList[i - 1], "Obj Pos Number: " + itemList[i - 1].ObjectPositionNumber + "Texutre ID: " + itemList[i - 1].TextureNumber.ToString() + "ERASED ITEM");
                    return;
                }

                itemList[i - 1].TextureNumber = textureID;
                itemList[i - 1].ObjectPositionNumber = i - 1;
                itemList[i - 1].ObjectType = selectedObject;
                itemList[i - 1].SpeedX = speedX;
                itemList[i - 1].SpeedY = speedY;
                itemList[i - 1].calculatePosition();
                itemList[i - 1].convertObjectType();
                toolTipList[i - 1].SetToolTip(pictureBoxList[i - 1], "Obj Pos Number: " + itemList[i - 1].ObjectPositionNumber + "Texutre ID: " + itemList[i - 1].TextureNumber.ToString() + ", Object Type: " + itemList[i - 1].ObjectOutput.ToString() + ", SpeedX: " + itemList[i - 1].SpeedX.ToString() + ", SpeedY: " + itemList[i - 1].SpeedY.ToString() + ", PositionX: " + itemList[i - 1].PositionX + ", Position Y: " + itemList[i - 1].PositionY);
            }
        }

        // update combobox 1 (type of item)
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e) {
            selectedObjectIndex = this.comboBox1.SelectedIndex;
            selectedObject = objectList[selectedObjectIndex];
            this.label1.Text = selectedObject;
        }
        // update combobox 2 (texture)
        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e) {
            textureID = this.comboBox2.SelectedIndex;            
        }
        // update speed used for enemies
        private void button1_Click(object sender, EventArgs e) {
            speedX = float.Parse(SpeedXBox.Text);
            speedY = float.Parse(SpeedYBox.Text);
        }
        // output text file
        private void button2_Click(object sender, EventArgs e) {
            outputFile();            
        }
        // add items in order to the textlist which is used for output
        private void updateTextList(ref List<Item>items, ref List<String> textList) {
            foreach (var item in items) {
                textList.Add(item.objectOutput());
            }
        }

        private void outputFile() {
            List<String> textList = new List<String>();
            List<Item> staticPlatforms = new List<Item>();
            List<Item> staticObjects = new List<Item>();
            List<Item> staticEnemies = new List<Item>();
            List<Item> movingEnemies = new List<Item>();

            outputTextFileName = this.outputFileBox.Text;
            levelID = this.IdTextBox.Text;

            System.IO.File.Delete(outputTextFileName+".txt");            
            itemList.Reverse(); // reverse the itemlist so it goes top to bottom - this seems to work better in game
            
            // break up the itemlist into specific items so they are grouped together in output - again this works better in game
            foreach (var item in itemList) {
                if ((item.TextureNumber == 0)) { // ignore and continue
                    continue;
                }
                else {
                    switch (item.ObjectType) {
                        case "Static Object":
                            staticObjects.Add(item);
                            break;
                        case "Static Platform":
                            staticPlatforms.Add(item);
                            break;
                        case "Enemy Static":
                            staticEnemies.Add(item);
                            break;
                        case "Enemy Move Horizontal":
                        case "Enemy Move Vertical":
                            movingEnemies.Add(item);
                            break;
                        default:
                            continue;
                    }
                }
            }
            // complete text list
            textList.Add(
                "num: " + itemList.Count() + // TODO: do a genuine count ignoring empty blocks!
                System.Environment.NewLine +
                "roomID:" + levelID);

            updateTextList(ref staticObjects, ref textList);
            updateTextList(ref staticPlatforms, ref textList);
            updateTextList(ref staticEnemies, ref textList);
            updateTextList(ref movingEnemies, ref textList);
            // write text list
            System.IO.File.WriteAllLines(outputTextFileName+".txt", textList.ToArray());
            textList.Clear();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e) {           
            DialogResult result = MessageBox.Show("Are you sure you want to start over? You will lose any unsaved work", "Reset?", MessageBoxButtons.YesNo);

            if (result == DialogResult.Yes) {
                itemList.Clear();
                textureList.Clear();
                objectList.Clear();
                pictureBoxList.Clear();
                objectComboBoxList.Clear();
                textureComboBoxList.Clear();
                textureID = 0;
                speedX = 0.0f;
                speedY = 0.0f;
                selectedObject = "";
                selectedObjectIndex = 0;
                this.comboBox1.Items.Clear();
                this.comboBox2.Items.Clear();
                resetEditor();
            }
            else if (result == DialogResult.No) {
                return;
            }            
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("JSB Level Editor - pre alpha", "JSB" , MessageBoxButtons.OK);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string outputfile = "level.bin";
            using (Stream stream = File.Open(outputfile, FileMode.Create))
            {
                var binaryOutput = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                binaryOutput.Serialize(stream, itemList);
            }
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string inputfile = "level.bin";

            using (Stream stream = File.Open(inputfile, FileMode.Open))
            {
                itemList.Clear();
                toolTipList.Clear();
                var binaryInput = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                itemList = (List<Item>)binaryInput.Deserialize(stream);

                for (int i = 0; i < itemList.Count(); i++)
                {
                    pictureBoxList[itemList[i].ObjectPositionNumber].Image = Image.FromFile(textureList[itemList[i].TextureNumber]);
                    // TODO: fix later
                    // toolTipList[itemList[i].ObjectPositionNumber].SetToolTip(pictureBoxList[i], "Obj Pos Number: " + itemList[i].ObjectPositionNumber + "Texutre ID: " + itemList[i].TextureNumber.ToString() + ", Object Type: " + itemList[i].ObjectOutput.ToString() + ", SpeedX: " + itemList[i].SpeedX.ToString() + ", SpeedY: " + itemList[i].SpeedY.ToString() + ", PositionX: " + itemList[i].PositionX + ", Position Y: " + itemList[i].PositionY);
                }
            }
        }
    }

    [Serializable]
    public class Item
    {
        const string STATIC_OBJECT = "s", ENEMY = "e", STATIC_PLATFORM = "t", STATIC_STAIRS = "l", ENEMY_MOVING = "m", ENEMY_STATIC = "n", PICK_UP = "u";
        private string _objectType;
        private string _objectTypeOutput;
        private int _textureNumber;        
        private int _objectPostionNumber;
        private float _speedX;
        private float _speedY;

        private int _positionX;
        private int _positionY;

        public Item(){}

        public string ObjectType { set { _objectType = value; } get { return _objectType; } }
        public int ObjectPositionNumber { set { _objectPostionNumber = value; } get { return _objectPostionNumber; } }
        public int TextureNumber { set { _textureNumber = value; } get { return _textureNumber; } }
        public string ObjectOutput { get { return _objectTypeOutput; } }
        public float SpeedX { get { return _speedX; } set { _speedX = value; } }
        public float SpeedY { get { return _speedY; } set { _speedY = value; } }
        public int PositionX { get { return _positionX; } }
        public int PositionY { get { return _positionY; } }

        public void convertObjectType() {
            switch (ObjectType) {
                case "Static Platform":
                    _objectTypeOutput = STATIC_PLATFORM;
                    break;
                case "Static Object":
                    _objectTypeOutput = STATIC_OBJECT;
                    break;
                case "Static Stairs":
                    _objectTypeOutput = STATIC_STAIRS;
                    break;
                case "Enemy Static":
                    _objectTypeOutput = ENEMY_STATIC;
                    break;
                case "Enemy Move Horizontal":
                    _objectTypeOutput = ENEMY_MOVING;
                    break;
                case "Enemy Move Vertical":
                    _objectTypeOutput = ENEMY_MOVING;
                    break;
                case "Erase":
                    _objectTypeOutput = "@";
                    _textureNumber = 0;
                    break;
                default:
                    _objectTypeOutput = "@";
                    _textureNumber = 0;
                    break;
            }
        }
        // convert the item number into a pixel number
        public void calculatePosition() {
            const int GROUND_HEIGHT = 638; // game groundheight
            _positionX = 0;
            _positionY = 0;
            int calcPos = 0;

            for (int i = 0; i < 921; i++) {
                if (_objectPostionNumber == 0) {                    
                    break;
                }        

                else if (i == _objectPostionNumber) {
                    break;
                }
                                
                else if (calcPos == 39) {
                    _positionX = 0;
                    _positionY += 32;
                    calcPos = 0;
                    continue;
                }
               
                _positionX += 32;
                calcPos++;
            }

            if (_positionY == 640) {
                _positionY = GROUND_HEIGHT; // hack because of the groundheight in game
            }
        }

        public string objectOutput() {
            if (_textureNumber == 0) { // ignore any textures with 0 in case anything slipped through (unlikely)
                return null;
            }

            string output = 
                "type:" + _objectTypeOutput +
                System.Environment.NewLine +
                "texture:" + _textureNumber.ToString() +
                System.Environment.NewLine +
                "px:" + _positionX.ToString() + ".0" +
                System.Environment.NewLine +
                "py:" + _positionY.ToString() + ".0" +
                System.Environment.NewLine +
                "sx:" + _speedX.ToString() +
                System.Environment.NewLine +
                "sy:" + _speedY.ToString() +
                System.Environment.NewLine +
                "end:0.0";
            return output;
        }
    }

    public class ComboBoxItem
    {
        public string Text { get; set; }

        public override string ToString() {
            return Text;
        }
    }
}
