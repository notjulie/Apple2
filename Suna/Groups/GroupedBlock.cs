﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Groups
{
   class GroupedBlock
   {
      private List<Group> groups = new List<Group>();

      public void Add(Group group)
      {
         groups.Add(group);
      }
   }
}