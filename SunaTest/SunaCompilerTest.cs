using System;
using System.IO;
using System.Reflection;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using Suna;
using Suna.Link;

namespace SunaTest
{
   [TestClass]
   public class SunaCompilerTest
   {
      [TestMethod]
      public void TestBeep()
      {
         LinkedModule linkedModule = CompileResource("Beep.suna");
         byte[] expected =
            new byte[] {
               0x20,
               0x3A,
               0xFF,
               0x20,
               0x3A,
               0xFF,
               0x20,
               0x3A,
               0xFF,
               0x60
            };
         var actual = linkedModule.GetImage();
         Assert.AreEqual(expected.Length, actual.Length);
         for (int i=0; i<expected.Length; ++i)
            Assert.AreEqual(expected[i], actual[i]);
      }

      /// <summary>
      /// Compiles the named resource
      /// </summary>
      /// <param name="resourceName"></param>
      private LinkedModule CompileResource(string resourceName)
      {
         // get the resource stream
         Assembly assembly = Assembly.GetExecutingAssembly();
         Stream stream = assembly.GetManifestResourceStream("SunaTest." + resourceName);

         // create a source module
         SourceModule sourceModule = new SourceModule(stream);

         // compile it
         SunaCompiler compiler = new SunaCompiler();
         return compiler.Compile(sourceModule);
      }
   }
}
