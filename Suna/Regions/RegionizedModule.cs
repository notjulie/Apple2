using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Regions
{
   /// <summary>
   /// A source module broken up into regions; currently the only purpose is to
   /// separate sections of JavaScript from sections of Suna.
   /// </summary>
   public class RegionizedModule
   {
      #region Types / Constants

      private enum Error
      {
         MissingEndJavascript
      }

      #endregion

      #region Private Fields

      private List<JavascriptRegion> javascriptRegions = new List<JavascriptRegion>();
      private List<SunaRegion> sunaRegions = new List<SunaRegion>();

      #endregion

      #region Constructor

      /// <summary>
      /// A source module that has been broken into regions
      /// </summary>
      /// <param name="sourceModule"></param>
      public RegionizedModule(SourceModule sourceModule)
      {
         Contract.Requires(sourceModule != null);

         SunaRegion currentSunaRegion = new SunaRegion();
         using (var reader = sourceModule.OpenReader())
         {
            for (; ; )
            {
               // read
               string line = reader.ReadLine();
               if (line == null)
                  break;

               // if this is a #beginJavascript line then do so
               if (line.Trim().IndexOf("#beginJavascript", StringComparison.InvariantCultureIgnoreCase) == 0)
               {
                  sunaRegions.Add(currentSunaRegion);
                  currentSunaRegion = new SunaRegion();

                  JavascriptRegion jsRegion = new JavascriptRegion();
                  for (; ; )
                  {
                     // read
                     string jsLine = reader.ReadLine();
                     if (jsLine == null)
                        throw new CompileException(Error.MissingEndJavascript);


                     if (jsLine.Trim().IndexOf("#endJavascript", StringComparison.InvariantCultureIgnoreCase) == 0)
                        break;

                     jsRegion.AddLine(jsLine);
                  }
                  this.javascriptRegions.Add(jsRegion);
               }
               else
               {
                  currentSunaRegion.AddLine(line);
               }
            }
         }

         // add the last region
         sunaRegions.Add(currentSunaRegion);
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the JavaScript regions
      /// </summary>
      public ICollection<JavascriptRegion> JavascriptRegions
      {
         get
         {
            return this.javascriptRegions;
         }
      }

      /// <summary>
      /// Gets the SUna regions
      /// </summary>
      public ICollection<SunaRegion> SunaRegions
      {
         get
         {
            return sunaRegions;
         }
      }

      #endregion
   }
}
