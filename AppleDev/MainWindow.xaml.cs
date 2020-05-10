﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace AppleDev
{
   /// <summary>
   /// Interaction logic for MainWindow.xaml
   /// </summary>
   public partial class MainWindow : Window
   {
      /// <summary>
      /// Initializes a new instance of class MainWindow
      /// </summary>
      public MainWindow()
      {
         // normal component initialization
         InitializeComponent();

         // event handlers
         windowMenu.SubmenuOpened += WindowMenu_SubmenuOpened;
         emulatorItem.Click += EmulatorItem_Click;
         programText.TextChanged += ProgramText_TextChanged;

         // grab the program text from the settings
         programText.Text = AppleDev.Properties.Settings.Default.ProgramText;
      }

      private void ProgramText_TextChanged(object sender, TextChangedEventArgs e)
      {
         AppleDev.Properties.Settings.Default.ProgramText = programText.Text;
      }

      private void EmulatorItem_Click(object sender, RoutedEventArgs e)
      {
         AppleWin.Managed.AppleWinThread.Show(!emulatorItem.IsChecked);
      }

      private void WindowMenu_SubmenuOpened(object sender, RoutedEventArgs e)
      {
         emulatorItem.IsChecked = AppleWin.Managed.AppleWinThread.IsShowing();
      }
   }
}
