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

namespace JSB_LevelEditor
{
    public partial class Form1 : Form
    {
        const int totalPixels = 921;
        ObservableCollection<PictureBox> pictureBoxList = new ObservableCollection<PictureBox>();
        List<Item> itemList = new List<Item>();
        List<String> textureList = new List<String>();
        List<String> objectList = new List<string>();
        List<String> textList = new List<String>();

        ObservableCollection<ToolTip> toolTipList = new ObservableCollection<ToolTip>();
        
        static int textureID = 0;
        static float speedX = 0.0f;
        static float speedY = 0.0f;

        static string selectedObject = "";
        static int selectedObjectIndex = 0;
                
        string defaultTexture = "Default_Blank.png";

        List<ComboBoxItem> objectComboBoxList = new List<ComboBoxItem>();
        List<ComboBoxItem> textureComboBoxList = new List<ComboBoxItem>();

        public Form1() {           
            InitializeComponent();
            for (int i = 1; i < totalPixels; i++) {
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
            
            foreach (var item in pictureBoxList) {
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

            for(int i = 0; i < objectList.Count(); i++)
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

        private void PixelClickEvent(object sender, MouseEventArgs e) {
            
            if (sender is PictureBox pictureBox) {
                pictureBox.Image = Image.FromFile(textureList[textureID]);                
                int i = Int32.Parse(pictureBox.Tag.ToString());
                this.label2.Text = itemList[i - 1].TextureNumber.ToString();
                if (selectedObject == "Erase")
                {   
                    itemList[i - 1].TextureNumber = 0;
                    itemList[i - 1].ObjectPositionNumber = i - 1;
                    itemList[i - 1].ObjectType = selectedObject;                
                    toolTipList[i - 1].SetToolTip(pictureBoxList[i - 1], "Obj Pos Number: " + itemList[i - 1].ObjectPositionNumber + "Texutre ID: " + itemList[i - 1].TextureNumber.ToString() + ", Object Type: " + itemList[i - 1].ObjectOutput.ToString() + ", SpeedX: " + itemList[i - 1].SpeedX.ToString() + ", SpeedY: " + itemList[i - 1].SpeedY.ToString() + ", PositionX: " + itemList[i - 1].PositionX + ", Position Y: " + itemList[i - 1].PositionY);
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

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            selectedObjectIndex = this.comboBox1.SelectedIndex;
            selectedObject = objectList[selectedObjectIndex];
            this.label1.Text = selectedObject;

        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            textureID = this.comboBox2.SelectedIndex;            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            speedX = float.Parse(SpeedXBox.Text);
            speedY = float.Parse(SpeedYBox.Text);
        }


        private void button2_Click(object sender, EventArgs e)
        {
            outputFile();            
        }

        private void outputFile()
        {
            System.IO.File.Delete("levelEditorTest.txt");            
            itemList.Reverse();
            foreach (var item in itemList)
            {
                if ((item.TextureNumber == 0))
                {
                    continue;                    
                }
                else
                {
                    textList.Add(item.objectOutput());
                }
            }

            System.IO.File.WriteAllLines("levelEditorTest.txt", textList.ToArray());
            textList.Clear();
        }
    }

    public class Item
    {
        const char STATIC_OBJECT = 's', ENEMY = 'e', STATIC_PLATFORM = 't', STATIC_STAIRS = 'l', ENEMY_MOVING = 'm', ENEMY_STATIC = 'n', PICK_UP = 'u';
        private string _objectType;
        private char _objectTypeOutput;
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
        public char ObjectOutput { get { return _objectTypeOutput; } }
        public float SpeedX { get { return _speedX; } set { _speedX = value; } }
        public float SpeedY { get { return _speedY; } set { _speedY = value; } }
        public int PositionX { get { return _positionX; } }
        public int PositionY { get { return _positionY; } }

        public void convertObjectType()
        {
            switch (ObjectType)
            {
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
                    _objectTypeOutput = '@';
                    _textureNumber = 0;
                    break;
                default:
                    _objectTypeOutput = '@';
                    _textureNumber = 0;
                    break;
            }
        }

        public void calculatePosition()
        {
            _positionX = 0;
            _positionY = 0;
            int calcPos = 0;

            for (int i = 0; i < 921; i++)
            {
                if (_objectPostionNumber == 0)
                {                    
                    break;
                }        

                else if (i == _objectPostionNumber)
                {
                    break;
                }
                                
                else if (calcPos == 39)
                {
                    _positionX = 0;
                    _positionY += 32;
                    calcPos = 0;
                    continue;
                }
               
                _positionX += 32;
                calcPos++;
            }

            if (_positionY == 640)
            {
                _positionY = 638; // hack because of the groundheight in game
            }
        }

        public string objectOutput()
        {
            if (_textureNumber == 0)
            {
                return null;
            }

            string output = "type:" + _objectTypeOutput +
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

        public override string ToString()
        {
            return Text;
        }
    }
}
