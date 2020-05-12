//  SORT. Sort a linear singly-linked list of INTs.

class Sort
{

//  NODE. A node in a linear singly linked list of INTs.

  private static class Node
  {
    private int  number;  //  The INT in the node, duh.
    private Node next;    //  The NODE that follows this one, or NULL.

//  Constructor. Initialize a new NODE with NUMBER and NEXT.

    private Node(int number, Node next)
    {
      this.number = number;
      this.next = next;
    }
  }

//  MAKE NODES. Return a list of NODEs that contains INTs from NUMBERS in order
//  of their appearance.

  private static Node makeNodes(int ... numbers)
  {
    if (numbers.length > 0)
    {
      Node first = new Node(numbers[0], null);
      Node last  = first;
      for (int index = 1; index < numbers.length; index += 1)
      {
        last.next = new Node(numbers[index], null);
        last = last.next;
      }
      return first;
    }
    else
    {
      return null;
    }
  }

//  WRITE NODES. Write the INTs from a list of NODEs in paired square brackets,
//  separated by commas, with a newline at the end.

  private static void writeNodes(Node nodes)
  {
    System.out.print('[');
    if (nodes != null)
    {
      System.out.print(nodes.number);
      nodes = nodes.next;
      while (nodes != null)
      {
        System.out.print(", ");
        System.out.print(nodes.number);
        nodes = nodes.next;
      }
    }
    System.out.println(']');
  }

//  SORT NODES. Sort UNSORTED, a list of NODEs, into nondecreasing order of its
//  NUMBER slots, without making new NODEs.

  private static Node sortNodes(Node unsorted)
  {
    if (unsorted == null)
    {
      return unsorted;
    }
    else if (unsorted.next == null)
    {
      return unsorted;
    }
    else
    {
      Node right = null;
      Node left = null;
      Node sorted = null;
      Node temp = null;
      while (unsorted != null)
      {
        temp = unsorted;
        if (left == null)
        {
          left = new Node(temp.number,null);
        }
        else
        {
          left = new Node(temp.number,left);
        }
        unsorted = unsorted.next;
        if (unsorted != null)
        {
          temp = unsorted;
          if (right == null)
          {
            right = new Node(temp.number,null);
          }
          else
          {
            right = new Node(temp.number,right);
          }
          unsorted = unsorted.next;
        }
      }
        left = sortNodes(left);
        right = sortNodes(right);
        while (right!= null && left != null)
        {
          if (left.number < right.number)
          {
            if (sorted == null)
            {
              sorted = new Node(left.number,null);
              temp = sorted;
            }
            else
            {
              temp.next = new Node(left.number,null);
              temp = temp.next;
            }
            left = left.next;
          }

          else
          {
            if (sorted == null)
            {
              sorted = new Node(right.number,null);
              temp = sorted;
            }
            else
            {
              temp.next = new Node(right.number,null);
              temp = temp.next;
            }
            right = right.next;
          }
        }
        if (right == null)
        {
            temp.next = left;
        }
        else if (left == null)
        {
            temp.next = right;
        }
    return sorted;
  }
}

//  MAIN. Run some examples. The comments show what must be printed.

  public static void main(String [] args)
  {
    writeNodes(sortNodes(makeNodes()));      //  []
    writeNodes(sortNodes(makeNodes(1)));     //  [1]
    writeNodes(sortNodes(makeNodes(1, 2)));  //  [1, 2]
    writeNodes(sortNodes(makeNodes(2, 1)));  //  [1, 2]

    writeNodes(sortNodes(makeNodes(1, 2, 3, 4, 5, 6, 7, 8, 9)));
    //  [1, 2, 3, 4, 5, 6, 7, 8, 9]

    writeNodes(sortNodes(makeNodes(9, 8, 7, 6, 5, 4, 3, 2, 1)));
    //  [1, 2, 3, 4, 5, 6, 7, 8, 9]

    writeNodes(sortNodes(makeNodes(3, 1, 4, 5, 9, 2, 6, 8, 7)));
    //  [1, 2, 3, 4, 5, 6, 7, 8, 9]
  }
}
