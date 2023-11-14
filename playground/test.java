/*
 *  Copyright (C) Chaperon. All rights reserved.
 *  -------------------------------------------------------------------------
 *  This software is published under the terms of the Apache Software License
 *  version 1.1, a copy of which has been included  with this distribution in
 *  the LICENSE file.
 */

package net.sourceforge.chaperon.model.grammar;

import java.io.Serializable;
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.Vector;

import net.sourceforge.chaperon.common.IntegerList;
import net.sourceforge.chaperon.model.Violations;
import net.sourceforge.chaperon.model.symbol.Symbol;
import net.sourceforge.chaperon.model.symbol.SymbolList;
import net.sourceforge.chaperon.model.symbol.SymbolSet;
import net.sourceforge.chaperon.model.symbol.Nonterminal;
import net.sourceforge.chaperon.model.symbol.Terminal;

/**
 * This class contains a grammar for building a parser table
 *
 * @author <a href="mailto:stephan@apache.org">Stephan Michels</a>
 * @version CVS $Id: test3.java,v 1.3 2003/01/21 22:37:19 benedikta Exp $
 */
public class Grammar implements Serializable, Cloneable
{
	// Start symbol
	private Nonterminal startsymbol = null;

	// Productions
	private Vector      productions = new Vector();

  private Hashtable   priorities = new Hashtable();
  private Hashtable   associativities = new Hashtable();

  private String      location = null;

	/**
	 * Creates a empty grammar
	 */
	public Grammar()
	{
	}

  /**
   * Add a production to this list
   *
   * @param production Production, which should added
   *
   * @return Index of the production in this list
   */
  public int addProduction(Production production)
  {
    if (production==null)
      throw new NullPointerException();

    productions.addElement(production);
    return productions.size() - 1;
  }

  /**
   * Add a list of productions
   *
   * @param list Array of productions
   */
  public void addProduction(Production[] list)
  {
    for (int i = 0; i < list.length; i++)
    {
      try
      {
        addProduction((Production)list[i].clone());
      } catch (CloneNotSupportedException cnse)
      {
        throw new IllegalArgumentException("Could not clone token:"+cnse.getMessage());
      }
    }
  }

  /**
   * Removes a production by an index from this list
   *
   * @param index Index of the production, which should be removed
   */
  public void removeProduction(int index)
  {
    productions.removeElementAt(index);
  }

  /**
   * Set a production by an index
   *
   * @param index The index, at which the production be inserted
   * @param production Production
   */
  public void setProduction(int index,
                            Production production)
                              throws IndexOutOfBoundsException
  {
    if ((index < 0) || (index > productions.size()))
      throw new IndexOutOfBoundsException();
    productions.setElementAt(production, index);
  }

  /**
   * Return a production giving by an index
   *
   * @param index Index of the Production
   *
   * @return Production
   */
  public Production getProduction(int index) throws IndexOutOfBoundsException
  {
    if ((index < 0) || (index > productions.size()))
      throw new IndexOutOfBoundsException();

    return (Production) productions.elementAt(index);
  }

 /**
  * Returns all production for given non terminal symbol
  *
  * @param ntsymbol Non terminal symbol
  *
  * @return List of indices from found productions
  */
  public IntegerList getProductionList(Symbol ntsymbol)
  {
    IntegerList list = new IntegerList();
    int i;

    for (i = 0; i < getProductionCount(); i++)
    {
      if (getProduction(i).getSymbol().equals(ntsymbol))
        list.add(i);
    }
    return list;
  }

  /**
   * Returns the count of productions in the list
   *
   * @return Count of productions
   */
  public int getProductionCount()
  {
    return productions.size();
  }

 /**
  * Return the index of a production
  *
  * @param production Production
  *
  * @return Index of the Production
  */
  public int indexOf(Production production)
  {
    for (int i = 0; i < productions.size(); i++)
      if (((Production) productions.elementAt(i)).equals(production))
        return i;
    return -1;
  }

 /**
  * Return the index of the next production, which found
  * by a non terminal symbol
  *
  * @param ntsymbol Non terminal symbol
  *
  * @return Production, which found
  */
  public int indexOf(Symbol ntsymbol)
  {
    for (int i = 0; i < productions.size(); i++)
      if (((Production) productions.elementAt(i)).getSymbol().equals(ntsymbol))
        return i;
    return -1;
  }

  /**
   * If the list contains a production
   *
   * @param production Production
   *
   * @return True, if the list contains the production
   */
  public boolean contains(Production production)
  {
    return (indexOf(production) != -1);
  }

  /**
   * If the list contains a production with a special symbol
   *
   * @param ntsymbol Non terminal symbol
   *
   * @return True, if the list contains a production with the symbol
   */
  public boolean contains(Symbol ntsymbol)
  {
    return (indexOf(ntsymbol) != -1);
  }

  /**
   * Removes all productions from this list
   */
  public void removeAllProduction()
  {
    productions.removeAllElements();
  }

 /**
  * Returns a enumeration of the productions
  * in this list.
  *
  * @return Enumeration of the productions
  */
  public Enumeration enumerateProduction()
  {
    return productions.elements();
  }

  /**
   * Returns the content of this list
   *
   * @return Array of the tokens
   */
  public Production[] getProduction()
  {
    int size = productions.size();
    Production[] mArray = new Production[size];

    for (int index = 0; index < size; index++)
      mArray[index] = (Production) productions.elementAt(index);
    return mArray;
  }

  /**
   * Replace the content of this list by the content of an array
   *
   *
   * @param productionArray
   */
  public void setProduction(Production[] productionArray)
  {
    productions.removeAllElements();
    for (int i = 0; i < productionArray.length; i++)
      productions.addElement(productionArray[i]);
  }

  public void setPriority(Terminal terminal, int priority)
  {
    if (terminal==null)
      throw new NullPointerException();

    priorities.put(terminal, new Integer(priority));
  }

  public int getPriority(Terminal terminal)
  {
    Integer priority = (Integer)priorities.get(terminal);
    if (priority==null)
      return 0;
    return priority.intValue();
  }

  public int getPriority(Production production)
  {
    if (!contains(production))
      return 0;

    if (production.getPrecedence() != null)
      return getPriority(production.getPrecedence());

    SymbolList definition = production.getDefinition();
    for (int i = definition.getSymbolCount() - 1; i >= 0; i--)
      if (definition.getSymbol(i) instanceof Terminal)
      {
        int priority =  getPriority((Terminal)definition.getSymbol(i));
        if (priority==0)
          return getProductionCount()-indexOf(production);
        return priority;
      }

    return getProductionCount()-indexOf(production);
  }

  public void setAssociativity(Terminal terminal, Associativity assoc)
  {
    if (terminal==null)
      throw new NullPointerException();

    associativities.put(terminal, assoc);
  }

  public Associativity getAssociativity(Terminal terminal)
  {
    Associativity assoc = (Associativity)associativities.get(terminal);
    if (assoc==null)
      return Associativity.NONASSOC;
    return assoc;
  }

  public Associativity getAssociativity(Production production)
  {
    if (!contains(production))
      return Associativity.NONASSOC;

    if (production.getPrecedence() != null)
      return getAssociativity(production.getPrecedence());

    SymbolList definition = production.getDefinition();
    for (int i = definition.getSymbolCount() - 1; i >= 0; i--)
      if (definition.getSymbol(i) instanceof Terminal)
        return getAssociativity((Terminal)definition.getSymbol(i));

    return Associativity.NONASSOC;
  }

  /**
   * Return all used symbols in this grammar
   *
   * @return Set of symbols
   */
  public SymbolSet getSymbols()
  {
    SymbolSet set = new SymbolSet();

    for (int i = 0; i < getProductionCount(); i++)
      set.addSymbol(getProduction(i).getSymbols());
    return set;
  }

	/**
	 * Set the a symbol a start symbol
	 *
	 * @param startsymbol
	 */
	public void setStartSymbol(Nonterminal startsymbol)
	{
		this.startsymbol = startsymbol;
	}

	/**
	 * Returns the start symbol
	 *
	 * @return Start symbol
	 */
	public Nonterminal getStartSymbol()
	{
		return startsymbol;
	}

 /**
  * Set the location from the input source.
  *
  * @param location Location in the input source.
  */
  public void setLocation(String location)
  {
    this.location = location;
  }

 /**
  * Returns the location from the input source.
  *
  * @return Location in the input source.
  */
  public String getLocation()
  {
    return location;
  }

  /**
   * This method validates the grammer
   *
   * @return Return a list of violations, if this
   *         object isn't correct
   */
  public Violations validate()
  {
    Violations violations = new Violations();
    if (startsymbol==null)
      violations.addViolation("Start symbol is not defined", location);

    if (getProductionCount() <= 0)
      violations.addViolation("No productions are defined", location);

    for (Enumeration e = productions.elements(); e.hasMoreElements() ;)
      violations.addViolations(((Production)e.nextElement()).validate());

    SymbolSet ntsymbols = getSymbols().getNonterminals();
    for (int i = 0; i < ntsymbols.getSymbolCount(); i++)
      if (!contains(ntsymbols.getSymbol(i)))
        violations.addViolation("Nonterminal symbol \"" + ntsymbols.getSymbol(i) + "\"" +
                                "is not defined through a production", location);

    return violations;
  }

	/*
	 *
	 *
	 * @return
	 */
	public String toString()
	{
    StringBuffer buffer = new StringBuffer();

    buffer.append(super.toString());
    buffer.append("\n");

    buffer.append("Terminal symbols:\n");
    SymbolSet tsymbols = getSymbols().getTerminals();
    for (int i = 0; i < tsymbols.getSymbolCount(); i++)
    {
      buffer.append(String.valueOf(i));
      buffer.append(".Terminal: ");
      buffer.append(tsymbols.getSymbol(i));
      buffer.append(" Priority=");
      buffer.append(String.valueOf(getPriority((Terminal)tsymbols.getSymbol(i))));
      buffer.append(" Associativity=");
      buffer.append(String.valueOf(getAssociativity((Terminal)tsymbols.getSymbol(i))));
      buffer.append("\n");
    }

    buffer.append("Produktions:\n");
    for (int i = 0; i < getProductionCount(); i++)
    {
      buffer.append(String.valueOf(i));
      buffer.append(".Production: ");
      buffer.append(getProduction(i).toString());
      buffer.append("\n");
    }

    buffer.append("\n");

		return buffer.toString();
	}

	/**
	 * Creates a clone of a grammar.
	 *
	 * @return
	 *
	 * @throws CloneNotSupportedException
	 */
	public Object clone() throws CloneNotSupportedException
	{
		Grammar clone = new Grammar();

		clone.startsymbol = startsymbol;
    for (int i = 0; i < productions.size(); i++)
      clone.addProduction((Production) ((Production) productions.elementAt(i)).clone());

    clone.location = location;

		return clone;
	}
}
