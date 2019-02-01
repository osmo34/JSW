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
// fix item count on output
// Add support for pickups
// consider play from editor

namespace JSB_LevelEditor
{
    public partial class Form1 : Form
    {
        bool firstRun = true;
        const int totalPixels = 921;        
        ObservableCollection<PictureBox> pictureBoxList = new ObservableCollection<PictureBox>();
        List<Item> itemList = new List<Item>();
        List<Item> stairList = new List<Item>();

        List<String> textureList = new List<String>();
        List<String> objectList = new List<string>();     

        ObservableCollection<ToolTip> toolTipList = new ObservableCollection<ToolTip>();

        string textureFileName = "textures.txt";
        string objectTypesFileName = "object_types.txt";

        static int textureID = 0;
        static float speedX = 0.0f;
        static float speedY = 0.0f;
        static float clampXLeft = 0.0f;
        static float clampXRight = 0.0f;
        static float clampYTop = 0.0f;
        static float clampYBottom = 0.0f;

        // stairs
        static float stairsBottomX = 0.0f;
        static float stairsBottomY = 0.0f;
        static float stairsTopX = 0.0f;
        static float stairsTopY = 0.0f;

        static string selectedObject = "";
        static int selectedObjectIndex = 0;

        static int selectedStairsIndex = 0;
                
        string defaultTexture = "Default_Blank.png";

        List<ComboBoxItem> objectComboBoxList = new List<ComboBoxItem>();
        List<ComboBoxItem> textureComboBoxList = new List<ComboBoxItem>();
        List<ComboBoxItem> stairsComboBoxList = new List<ComboBoxItem>();

        static string outputTextFileName = "output";
        static string levelID = "0";
        static string levelIDVertical = "0"; // determines room height in the world 

        string currentFileName;
        bool isSaved = false;

        bool isOutput = false;

        public Form1() {           
            InitializeComponent();
            resetEditor();
            firstRun = false;
            openFileDialog1.Filter = "Level files (*.bin)|*.bin|All files (*.*)|*.*";
            saveFileDialog1.Filter = "Level files (*.bin)|*.bin|All files (*.*)|*.*";
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

            textureList = UpdateEditorList(textureFileName);
            objectList = UpdateEditorList(objectTypesFileName);        
            
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
            this.comboBox2.SelectedIndex = 0;

            isOutput = false;
        }

        // populate lists
        private List<String> UpdateEditorList(string inputFile)
        {
            System.IO.StreamReader file = new System.IO.StreamReader(inputFile);
            List<string> list = new List<string>();
            while (!file.EndOfStream)
            {
                string word = file.ReadLine();
                string[] words = word.Split(new string[] { System.Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries);
                foreach (string t in words)
                {                    
                    list.Add(t);
                }
            }
            file.Close();
            return list;
        }

        // event for clicking in a pixel. If it's erase then change texture number to 0 so it is ignored at output (0 is the player sprite in game so is never used)
        private void PixelClickEvent(object sender, MouseEventArgs e) {            
            if (sender is PictureBox pictureBox) {
                pictureBox.Image = Image.FromFile(textureList[textureID]);                
                int i = Int32.Parse(pictureBox.Tag.ToString());
                this.label2.Text = itemList[i - 1].TextureNumber.ToString();
                // overcomplicated if logic to ensure we don't create a new object in error!
                if (this.itemList[i - 1].ObjectType == null) {
                    if (selectedObject == "Erase") {
                        return;
                    }
                    else {
                        CreateObject(i);
                    }
                }
                else {
                    if (selectedObject == "Erase") {
                        itemList[i - 1].TextureNumber = 0;
                        itemList[i - 1].ObjectPositionNumber = i - 1;
                        itemList[i - 1].ObjectType = null;
                        toolTipList[i - 1].SetToolTip(pictureBoxList[i - 1], "Obj Pos Number: " + itemList[i - 1].ObjectPositionNumber + "Texutre ID: " + itemList[i - 1].TextureNumber.ToString() + "ERASED ITEM");
                        return;
                    }
                    else {
                        CreateObject(i);
                    }
                }
            }
        }

        private void CreateObject(int i)
        {
            this.itemList[i - 1].TextureNumber = textureID;
            this.itemList[i - 1].TextureFileName = textureList[textureID];
            this.itemList[i - 1].ObjectPositionNumber = i - 1;
            this.itemList[i - 1].ObjectType = selectedObject;
            this.itemList[i - 1].SpeedX = speedX;
            this.itemList[i - 1].SpeedY = speedY;
            this.itemList[i - 1].clampXLeft = clampXLeft;
            this.itemList[i - 1].clampXRight = clampXRight;
            this.itemList[i - 1].clampYTop = clampYTop;
            this.itemList[i - 1].clampYBottom = clampYBottom;

            this.itemList[i - 1].calculatePosition();
            this.itemList[i - 1].convertObjectType();
            toolTipList[i - 1].SetToolTip(pictureBoxList[i - 1], "Obj Pos Number: " + itemList[i - 1].ObjectPositionNumber + "Texutre ID: " + itemList[i - 1].TextureNumber.ToString() + ", Object Type: " + itemList[i - 1].ObjectOutput.ToString() + ", SpeedX: " + itemList[i - 1].SpeedX.ToString() + ", SpeedY: " + itemList[i - 1].SpeedY.ToString() + ", PositionX: " + itemList[i - 1].PositionX + ", Position Y: " + itemList[i - 1].PositionY);            
        }

        // stairs
        private void CreateObject()
        {
            stairsBottomX = float.Parse(this.StairBottomXInputBox.Text);
            stairsBottomY = float.Parse(this.StairBottomYInputBox.Text);
            stairsTopX = float.Parse(this.StairTopXInputBox.Text);
            stairsTopY = float.Parse(this.StairTopYInputBox.Text);

            this.stairList.Add(new Item());
            int stairListCount = stairList.Count() - 1;

            this.stairList[stairListCount].TextureNumber = 1;
            this.stairList[stairListCount].TextureFileName = "ground_01.png"; // random texture to work in game, this will of course not be rendered
            this.stairList[stairListCount].ObjectPositionNumber = stairListCount;
            this.stairList[stairListCount].ObjectType = "Static Stairs";
            this.stairList[stairListCount].SpeedX = 0;
            this.stairList[stairListCount].SpeedY = 0;
            this.stairList[stairListCount].clampXLeft = 0;
            this.stairList[stairListCount].clampXRight = 0;
            this.stairList[stairListCount].clampYTop = 0;
            this.stairList[stairListCount].clampYBottom = 0;

            this.stairList[stairListCount].stairsBottomX = stairsBottomX;
            this.stairList[stairListCount].stairsBottomY = stairsBottomY;
            this.stairList[stairListCount].stairsTopX = stairsTopX;
            this.stairList[stairListCount].stairsTopY = stairsTopY;

            this.stairList[stairListCount].convertObjectType();

            // update combo box
            stairsComboBoxList.Add(new ComboBoxItem());
            int nextPosition = stairsComboBoxList.Count() - 1;
            stairsComboBoxList[nextPosition].Text = stairListCount.ToString();
            this.stairListComboBox.Items.Add(stairsComboBoxList[nextPosition]);
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
            this.texturePreview.SizeMode = PictureBoxSizeMode.StretchImage;
            this.texturePreview.Image = Image.FromFile(textureList[textureID]);            
        }
        // update stair combo list
        private void stairListComboBox_SelectedIndexChanged(object sender, EventArgs e) {
            selectedStairsIndex = this.stairListComboBox.SelectedIndex;
        }

        // update speed used for enemies
        private void button1_Click(object sender, EventArgs e) {
            speedX = float.Parse(SpeedXBox.Text);
            speedY = float.Parse(SpeedYBox.Text);
            clampXLeft = float.Parse(clampLeftBox.Text);
            clampXRight = float.Parse(clampRightBox.Text);
            clampYTop = float.Parse(clampTopBox.Text);
            clampYBottom = float.Parse(clampBottomBox.Text);
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
            isOutput = true;
            List<String> textList = new List<String>();
            List<Item> staticPlatforms = new List<Item>();
            List<Item> staticObjects = new List<Item>();
            List<Item> staticEnemies = new List<Item>();
            List<Item> movingEnemies = new List<Item>();
            List<Item> scenery = new List<Item>();
            List<Item> stairs = new List<Item>();

            List<Item> itemListDuplicate = new List<Item>(itemList); // we must duplicate itemlist, originally work was being done on the main list causing the erase bug - as it would reset itself!
            List<Item> stairListDuplicate = new List<Item>(stairList);

            outputTextFileName = this.outputFileBox.Text;
            levelID = this.IdTextBox.Text;
            levelIDVertical = this.idTextV_Box.Text;

            System.IO.File.Delete(outputTextFileName+".txt");            
            itemListDuplicate.Reverse(); // reverse the itemlist so it goes top to bottom - this seems to work better in game            
            
            // break up the itemlist into specific items so they are grouped together in output - again this works better in game
            foreach (var item in itemListDuplicate) {
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
                        case "Scenery":
                            scenery.Add(item);
                            break;                        
                        default:
                            continue;
                    }
                }
            }

            // go through stair list
            foreach (var item in stairListDuplicate) {
                stairs.Add(item);
            }

            // complete text list
            textList.Add(
                "num:" + itemListDuplicate.Count() + // TODO: do a genuine count ignoring empty blocks!
                System.Environment.NewLine +
                "roomID:" + levelID + 
                System.Environment.NewLine + 
                "roomIDVertical:" + levelIDVertical);

            updateTextList(ref scenery, ref textList);
            updateTextList(ref staticObjects, ref textList);
            updateTextList(ref staticPlatforms, ref textList);
            updateTextList(ref staticEnemies, ref textList);
            updateTextList(ref movingEnemies, ref textList);
            updateTextList(ref stairs, ref textList);
            // write text list
            System.IO.File.WriteAllLines(outputTextFileName+".txt", textList.ToArray());
            textList.Clear();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e) {           
            DialogResult result = MessageBox.Show("Are you sure you want to start over? You will lose any unsaved work", "Reset?", MessageBoxButtons.YesNo);

            if (result == DialogResult.Yes) {
                isSaved = false;
                currentFileName = "";
                this.itemList.Clear();
                this.stairList.Clear();
                this.textureList.Clear();
                this.objectList.Clear();
                pictureBoxList.Clear();
                objectComboBoxList.Clear();
                textureComboBoxList.Clear();
                textureID = 0;
                speedX = 0.0f;
                speedY = 0.0f;
                clampXLeft = 0.0f;
                clampXRight = 0.0f;
                clampYTop = 0.0f;
                clampYBottom = 0.0f;
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

        private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
            Application.Exit();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e) {
            MessageBox.Show("JSB Level Editor - pre alpha", "JSB" , MessageBoxButtons.OK);
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e) {
            string outputfile;
            DialogResult result;

            if (!isSaved) {
                result = saveFileDialog1.ShowDialog();
                if (result == DialogResult.OK) {
                    outputfile = saveFileDialog1.FileName;
                    isSaved = true;
                    currentFileName = outputfile;
                }
                else {
                    return;
                }
            }
            else {
                outputfile = currentFileName;
            }
            
            using (Stream stream = File.Open(outputfile, FileMode.Create)) {
                var binaryOutput = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                binaryOutput.Serialize(stream, this.itemList);
                binaryOutput.Serialize(stream, this.stairList);
            }
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e) {
            string inputfile;            
            DialogResult result = openFileDialog1.ShowDialog();
            if (result == DialogResult.OK) {
                inputfile = openFileDialog1.FileName;
                currentFileName = inputfile;                
                using (Stream stream = File.Open(inputfile, FileMode.Open)) {                                        
                    this.itemList.Clear();
                    this.stairList.Clear();
                    this.textureList.Clear();
                    this.objectList.Clear();
                    pictureBoxList.Clear();
                    objectComboBoxList.Clear();
                    textureComboBoxList.Clear();
                    textureID = 0;
                    speedX = 0.0f;
                    speedY = 0.0f;
                    clampXLeft = 0.0f;
                    clampXRight = 0.0f;
                    clampYTop = 0.0f;
                    clampYBottom = 0.0f;
                    selectedObject = "";
                    selectedObjectIndex = 0;
                    this.comboBox1.Items.Clear();
                    this.comboBox2.Items.Clear();
                    resetEditor();
                    var binaryInput = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                    this.itemList = (List<Item>)binaryInput.Deserialize(stream);
                    this.stairList = (List<Item>)binaryInput.Deserialize(stream); // TODO: Comment out this line if using older files!

                    for (int i = 0; i < itemList.Count(); i++) {
                        pictureBoxList[itemList[i].ObjectPositionNumber].Image = Image.FromFile(textureList[itemList[i].TextureNumber]);
                        // TODO: fix later
                        //toolTipList[itemList[i].ObjectPositionNumber].SetToolTip(pictureBoxList[i], "Obj Pos Number: " + itemList[i].ObjectPositionNumber + "Texutre ID: " + itemList[i].TextureNumber.ToString() + ", Object Type: " + itemList[i].ObjectOutput.ToString() + ", SpeedX: " + itemList[i].SpeedX.ToString() + ", SpeedY: " + itemList[i].SpeedY.ToString() + ", PositionX: " + itemList[i].PositionX + ", Position Y: " + itemList[i].PositionY);
                    }
                }
                isSaved = true;
                isOutput = true;
            }
            else {
                return;
            }
        }

        private void buttonItemExplore_Click(object sender, EventArgs e) {
            ItemExplorer itemExplorer = new ItemExplorer(ref itemList);
            itemExplorer.Show();
        }

        // run compiler
        private void button3_Click(object sender, EventArgs e) {
            if (isOutput) {
                string input = outputTextFileName + ".txt";
                string output = outputTextFileName + ".jsb";
                string argument = input + " " + output;
                System.Diagnostics.Process process = new System.Diagnostics.Process();
                process.EnableRaisingEvents = false;
                process.StartInfo.FileName = "JSWlevelCompiler";
                process.StartInfo.Arguments = argument;
                process.Start();
            }
            else {
                MessageBox.Show("Please output a file first");
            }
        }

        private void createStairsButton_Click(object sender, EventArgs e) {
            CreateObject();
        }

        private void EraseStairsButton_Click(object sender, EventArgs e) {            
            stairList.RemoveAt(selectedStairsIndex);
            stairListComboBox.Items.RemoveAt(selectedStairsIndex);
        }        
    }

    [Serializable]
    public class Item
    {
        const string STATIC_OBJECT = "s", ENEMY = "e", STATIC_PLATFORM = "t", STATIC_STAIRS = "l", ENEMY_MOVING = "m", ENEMY_STATIC = "n", PICK_UP = "u", SCENERY = "#";
        private string _objectType;
        private string _objectTypeOutput;
        private int _textureNumber;        
        private int _objectPostionNumber;
        private float _speedX;
        private float _speedY;
        private float _clampXLeft;
        private float _clampXRight;
        private float _clampYTop;
        private float _clampYBottom;
        private float _stairsBottomX;
        private float _stairsBottomY;
        private float _stairsTopX;
        private float _stairsTopY;

        private int _positionX;
        private int _positionY;

        private string _textureFileName;

        public Item(){ _textureNumber = 0; }

        public string ObjectType { set { _objectType = value; } get { return _objectType; } }
        public int ObjectPositionNumber { set { _objectPostionNumber = value; } get { return _objectPostionNumber; } }
        public int TextureNumber { set { _textureNumber = value; } get { return _textureNumber; } }
        public string ObjectOutput { set { _objectTypeOutput = value; } get { return _objectTypeOutput; } }

        public float SpeedX { get { return _speedX; } set { _speedX = value; } }
        public float SpeedY { get { return _speedY; } set { _speedY = value; } }
        public float clampXLeft { get { return _clampXLeft; } set { _clampXLeft = value; } }
        public float clampXRight { get { return _clampXRight; } set { _clampXRight = value; } }
        public float clampYTop { get { return _clampYTop; } set { _clampYTop = value; } }
        public float clampYBottom { get { return _clampYBottom; } set { _clampYBottom = value; } }

        public string TextureFileName { get { return _textureFileName; } set { _textureFileName = value; } }
        
        public float stairsBottomX { get { return _stairsBottomX; } set { _stairsBottomX = value; } }
        public float stairsBottomY { get { return _stairsBottomY; } set { _stairsBottomY = value; } }
        public float stairsTopX { get { return _stairsTopX; } set { _stairsTopX = value; } }
        public float stairsTopY { get { return _stairsTopY; } set { _stairsTopY = value; } }

        public int PositionX { get { return _positionX; } }
        public int PositionY { get { return _positionY; } }

        public void convertObjectType() {
            switch (ObjectType) {

                case "Scenery":
                    _objectTypeOutput = SCENERY;
                    break;
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
                "texture:" + _textureFileName +
                System.Environment.NewLine +
                "px:" + _positionX.ToString() + ".0" +
                System.Environment.NewLine +
                "py:" + _positionY.ToString() + ".0" +
                System.Environment.NewLine +
                "sx:" + _speedX.ToString() +
                System.Environment.NewLine +
                "sy:" + _speedY.ToString() +
                System.Environment.NewLine +
                "clampXLeft:" + _clampXLeft.ToString() +
                System.Environment.NewLine +
                "clampXRight:" + _clampXRight.ToString() +
                System.Environment.NewLine +
                "clampYTop:" + _clampYTop.ToString() +
                System.Environment.NewLine +
                "clampYBottom:" + _clampYBottom.ToString() +
                System.Environment.NewLine +
                "stbx:" + _stairsBottomX.ToString() +
                System.Environment.NewLine +
                "stby:" + _stairsBottomY.ToString() +
                System.Environment.NewLine +
                "sttx:" + _stairsTopX.ToString() +
                System.Environment.NewLine +
                "stty:" + _stairsTopY.ToString() +
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
