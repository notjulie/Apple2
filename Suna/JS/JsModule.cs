using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Jint;

namespace Suna.JS
{
   class JsModule
   {
      private Engine jsEngine = new Jint.Engine();

      public void Execute(string jsString)
      {
         jsEngine.Execute(jsString);
      }

      public bool IsFunctionName(string name)
      {
         return jsEngine.Execute(name + " instanceof Function").GetCompletionValue().AsBoolean();
      }
   }
}
